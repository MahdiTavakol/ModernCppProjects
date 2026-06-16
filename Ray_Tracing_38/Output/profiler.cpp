#include "profiler.h"
#include <sstream>
#include <unordered_set>
#include <numeric>

profiler::profiler(communicator* para_, Logger* error_):
	para{para_},error{error_},
	program_start{std::chrono::steady_clock::now()}
{
}

void profiler::reset_program_timer()
{
    program_start = std::chrono::steady_clock::now();
}

void profiler::start_event(const std::string event_name_)
{
	Time start = std::chrono::steady_clock::now();
    timing_infos[event_name_] = TimingInfo{ start };
}

void profiler::stop_event(const std::string event_name_)
{
	Time end = std::chrono::steady_clock::now();
    auto itr = timing_infos.find(event_name_);
    if (itr == timing_infos.end())
    {
        throw std::invalid_argument("Cannot find event " + event_name_);
    }
    itr->second.myDuration += end - itr->second.myStart;
}


std::string profiler::start_thread_event(const std::string event_name_)
{
    // since memory allocation has some overhead I put this outside the lock
    std::unique_ptr<profiler> thread_profiler = std::make_unique<profiler>(para, error);
    std::string thread_name;
    {
        // I do not want two threads at the same time inquire the
        // thread_profilers otherwise both would have the same name!
        std::lock_guard<std::mutex> lk(mtx);
        int n_threads = thread_profilers.size();
        thread_name = std::to_string(n_threads);
        thread_profilers[thread_name] = std::move(thread_profiler);
    }
    thread_profilers[thread_name]->start_event(event_name_);
    return thread_name;
}

void profiler::start_thread_event(const std::string thread_name_, const std::string event_name_)
{
    auto iter = thread_profilers.find(thread_name_);
    if (iter == thread_profilers.end())
        throw std::invalid_argument("The thread you are looking for is not there!");
    auto& thread_profiler = iter->second;
    thread_profiler->start_event(event_name_);
}

void profiler::stop_thread_event(const std::string thread_name_, const std::string event_name_)
{
    auto iter = thread_profilers.find(thread_name_);
    if (iter == thread_profilers.end())
        throw std::invalid_argument("The thread you are looking for is not here!");
    auto& thread_profiler = iter->second;
    thread_profiler->stop_event(event_name_);
}

void profiler::update_average_info()
{
    // we stop the timer for the program duration now
    program_duration_min = 
        program_duration_avg =
            program_duration_max = 
                std::chrono::steady_clock::now() - program_start;

    for (auto& [event_name, timing_info] : timing_infos)
    {
        //looking for this event in the children profilers
        std::vector<Duration> durations;
        this->return_event_duration(event_name, durations);
        // finding the min, max and avg values
        auto min_itr = std::min_element(durations.begin(), durations.end());
        auto max_itr = std::max_element(durations.begin(), durations.end());
        Duration min_duration = *min_itr;
        Duration max_duration = *max_itr;

        int nThreads = static_cast<int>(durations.size());
        timing_info.nThreads = nThreads;
        //later one please use std library function
        Duration sum_duration = std::accumulate(
            durations.begin(), durations.end(), Duration{0}
        );
        Duration avg_duration = sum_duration / static_cast<double>(durations.size());


        timing_info.maxDuration = max_duration;
        timing_info.minDuration = min_duration;
        timing_info.avgDuration = avg_duration;

    }
}


void profiler::return_event_duration(const std::string event_name_, std::vector<Duration>& durations_)
{
    // this thread information
    auto itr = timing_infos.find(event_name_);
    if (itr != timing_infos.end()) {
        durations_.push_back(itr->second.myDuration);
    }
    // going through children
    for (auto& [thread_name, thread_profiler] : thread_profilers)
    {
        thread_profiler->return_event_duration(event_name_, durations_);
    }
}

void profiler::print_timing_info()
{
    if (error == nullptr)
        throw std::invalid_argument("The logger object has not been initialized yet!");


    // updating the avg, min and max info
    update_average_info();



    // I use two differnt streams for duration and average values
    std::stringstream duration_strm;
    std::stringstream percent_strm;

    constexpr int name_width = 16;
    constexpr int dur_width = 11;
    constexpr int pct_width = 11;
    constexpr int trd_width = 3;

    int msg_level = 0;

    error->print_message(std::string(52, '='), msg_level);
    error->print_message("Timing information", msg_level);
    error->print_message(std::string(52, '-'), msg_level);

    duration_strm << std::left << std::setw(name_width) << "Event name"
        << std::right << std::setw(2 * pct_width) << "Duration(ms) (min/avg/max)"
        << std::right << std::setw(pct_width + trd_width) << "n_threads" << std::endl;
    duration_strm << std::string(52, '.') << std::endl;

    percent_strm << std::left << std::setw(name_width) << "Event name"
        << std::right << std::setw(2 * pct_width) << "Percent (min/avg/max)"
        << std::right << std::setw(pct_width + trd_width) << "n_threads" << std::endl;;
    percent_strm << std::string(52, '.') << std::endl;


    for (const auto& [event_name, timing] : timing_infos)
    {
        int nThreads = timing.nThreads;

        double avg_duration_ms = timing.avgDuration.count();
        double min_duration_ms = timing.minDuration.count();
        double max_duration_ms = timing.maxDuration.count();

        double total_ms = program_duration_avg.count();

        double pct_avg = avg_duration_ms * 100.0 / total_ms;
        double pct_min = min_duration_ms * 100.0 / total_ms;
        double pct_max = max_duration_ms * 100.0 / total_ms;

        int short_name_width = name_width - 2;
        int name_rpts = (event_name.length()+ short_name_width -1) / short_name_width;
        std::string substr = event_name.substr(0, short_name_width);
        duration_strm << std::fixed << std::left << std::setw(name_width) << substr;
        percent_strm << std::fixed << std::left << std::setw(name_width) << substr;
        for (int i = 1; i < name_rpts; i++)
        {
            std::string substr = event_name.substr(i * short_name_width, short_name_width);
            duration_strm << std::endl << std::fixed << std::left << std::setw(name_width) << substr;
            percent_strm << std::endl <<std::fixed << std::left << std::setw(name_width) << substr;
        }



        duration_strm
            << std::fixed << std::setw(dur_width)
            << std::setprecision(3) << min_duration_ms
            << std::fixed << std::setw(dur_width)
            << std::setprecision(3) << avg_duration_ms
            << std::fixed << std::left << std::setw(dur_width)
            << std::setprecision(3) << max_duration_ms 
            << std::fixed << std::left << std::setw(trd_width)
            << nThreads << std::endl;

        percent_strm
            << std::fixed << std::setw(pct_width)
            << std::setprecision(3) << pct_min
            << std::fixed << std::setw(pct_width)
            << std::setprecision(3) << pct_avg
            << std::fixed << std::left << std::setw(dur_width)
            << std::setprecision(3) << pct_max << 
            std::fixed << std::left << std::setw(trd_width)
            << nThreads << std::endl;
    }

    duration_strm << std::string(52, '-') << std::endl;
    percent_strm << std::string(52, '.') << std::endl;
    percent_strm << "Program took "
        << program_duration_min
        << "/" << program_duration_avg
        << "/" << program_duration_max
        << std::endl;
    percent_strm << std::string(52, '=') << std::endl;

    error->print_message(duration_strm);
    error->print_message(percent_strm);

   
}

std::unique_ptr<profiler> profiler::average_multiple_profilers(std::vector<std::unique_ptr<profiler>>& profiler_vec_)
{
    communicator* para = profiler_vec_[0]->para;
    Logger* error = profiler_vec_[0]->error;

    std::unique_ptr<profiler> timer = std::make_unique<profiler>(para, error);
    std::vector<std::string> event_names;

    for (auto& profiler : profiler_vec_)
    {
        for (auto& [event_name, timing_info] : profiler->timing_infos)
        {
            event_names.push_back(event_name);
        }
    }

    std::unordered_map<std::string, TimingInfo> timing_info_map;

    for (auto& event_name : event_names)
    {
        Duration minDuration = Duration{ std::numeric_limits<double>::infinity()};
        Duration avgDuration = Duration{0};
        Duration maxDuration = Duration{0};
        int nThreads = 0;
        timing_info_map[event_name] = TimingInfo{minDuration,avgDuration,maxDuration,nThreads};
    }

    for (auto& profiler : profiler_vec_)
    {
        for (auto& [event_name, timing_info] : profiler->timing_infos)
        {
            Duration myMinDuration = timing_info.minDuration;
            Duration myAvgDuration = timing_info.avgDuration;
            Duration myMaxDuration = timing_info.maxDuration;
            int nThreads = timing_info.nThreads;

            Duration& allMinDur = timing_info_map[event_name].minDuration;
            Duration& allAvgDur = timing_info_map[event_name].avgDuration;
            Duration& allMaxDur = timing_info_map[event_name].maxDuration;
            int& allnThreads = timing_info_map[event_name].nThreads;
            timing_info_map[event_name].nProfilings++;
            timing_info_map[event_name].nThreads += nThreads;

            if (myMinDuration < allMinDur)
                allMinDur = myMinDuration;
            if (myMaxDuration > allMaxDur)
                allMaxDur = myMaxDuration;
            allAvgDur += myAvgDuration * static_cast<double>(nThreads);
        }
    }

    for (auto& [event_name, timing_info] : timing_info_map)
    {
        if (timing_info.nThreads > 0)
            timing_info.avgDuration /= static_cast<double>(timing_info.nThreads);
    }

    return timer;
}

