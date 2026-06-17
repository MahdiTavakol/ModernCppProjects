#include "profiler.h"
#include <sstream>
#include <unordered_set>
#include <numeric>

constexpr double eps = 1e-6;

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

void profiler::add_child_events()
{
    // event_name to the vector of event durations
    std::map<std::string, std::vector<Duration>> durations_map;

    // looping through children threads
    for (auto& [t_name, t_profiler] : thread_profilers)
    {
        // looping through its events
        for (auto& [e_name, t_info] : t_profiler->timing_infos)
        {
            // do we have such an event
            if (durations_map.find(e_name) == durations_map.end())
            {
                durations_map[e_name] = std::vector<Duration>();
            }
            // adding the duration of this event to the map
            durations_map[e_name].push_back(t_info.myDuration);
        }
    }


    // averaging them 
    for (auto& [e_name, d_vec] : durations_map)
    {
        if (timing_infos.find(e_name) != timing_infos.end())
        {
            std::string message = "Warning: name clash between the main program and threads ";
            message += "for the event: " + e_name;
            error->print_message(message, 0);
        }
        TimingInfo c_timing_info;
        c_timing_info.nThreads = d_vec.size();
        c_timing_info.minDuration = *std::min_element(d_vec.begin(), d_vec.end());
        c_timing_info.maxDuration = *std::max_element(d_vec.begin(), d_vec.end());
        c_timing_info.avgDuration = Duration{ 0 };
        c_timing_info.avgDuration = std::accumulate(d_vec.begin(), d_vec.end(), Duration{ 0 });
        c_timing_info.avgDuration /= static_cast<double>(d_vec.size());
        timing_infos[e_name] = c_timing_info;
    }
}

void profiler::print_timing_info()
{
    if (error == nullptr)
        throw std::invalid_argument("The logger object has not been initialized yet!");



    // we stop the timer for the program duration now
    program_duration_min =
        program_duration_avg =
            program_duration_max =
                std::chrono::steady_clock::now() - program_start;

    for (auto& [event_name, timing_info] : timing_infos)
    {
        timing_info.minDuration = timing_info.myDuration;
        timing_info.maxDuration = timing_info.myDuration;
        timing_info.avgDuration = timing_info.myDuration;
    }

    // updating the children timing info
    add_child_events();



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



       
        if (std::abs(max_duration_ms - min_duration_ms) <= eps) {
            duration_strm
                << std::fixed << std::setw(3*dur_width)
                << center(avg_duration_ms, 3 * dur_width)
                << std::fixed << std::left << std::setw(trd_width)
                << nThreads << std::endl;

            percent_strm
                << std::fixed << std::setw(3*pct_width)
                << center(pct_avg, 3 * pct_width)
                << std::fixed << std::left << std::setw(trd_width)
                << nThreads << std::endl;
        }
        else
        {
            std::string avg_duration_string = center(avg_duration_ms,  dur_width);
            std::string avg_pct_string = center(pct_avg, pct_width);
            duration_strm
                << center(min_duration_ms, dur_width)
                << center(avg_duration_ms, dur_width)
                << center(max_duration_ms, dur_width)
                << std::fixed << std::left << std::setw(trd_width)
                << nThreads << std::endl;

            percent_strm
                << center(pct_min,pct_width)
                << center(pct_avg,pct_width)
                << center(pct_max,pct_width)
                << std::fixed << std::left << std::setw(trd_width)
                << nThreads << std::endl;
        }
    }

    duration_strm << std::string(52, '-') << std::endl;
    percent_strm << std::string(52, '.') << std::endl;
    if (std::abs(program_duration_max.count() - program_duration_min.count()) < eps)
    {
        percent_strm << "Program took "
            <<  program_duration_avg
            <<  std::endl;
    }
    else
    {
        percent_strm << "Program took "
            << program_duration_min
            << "/" << program_duration_avg
            << "/" << program_duration_max
            << std::endl;
    }
    percent_strm << std::string(52, '=') << std::endl;

    error->print_message(duration_strm);
    error->print_message(percent_strm);

   
}

std::unique_ptr<profiler> profiler::average_multiple_profilers(std::vector<std::unique_ptr<profiler>>& profiler_vec_)
{
    communicator* para = profiler_vec_[0]->para;
    Logger* error = profiler_vec_[0]->error;

    std::unique_ptr<profiler> timer = std::make_unique<profiler>(para, error);

    // adding child events for each profiler
    for (auto& profiler : profiler_vec_)
    {
        profiler->add_child_events();
    }

    auto& event_map = timer->timing_infos;
    
    for (auto& profiler : profiler_vec_)
    {
        for (auto& [event_name, timing_info] : profiler->timing_infos)
        {
            // if event is not there add it
            if (event_map.find(event_name) == event_map.end())
            {
                TimingInfo newTI(0);
                newTI.minDuration = Duration{ std::numeric_limits<double>::infinity() };
                event_map[event_name] = newTI;
            }
            auto& event = event_map[event_name];
            Duration minDuration = timing_info.minDuration;
            Duration avgDuration = timing_info.avgDuration;
            Duration maxDuration = timing_info.maxDuration;
            int nThreads = timing_info.nThreads;
            Duration sumDuration = avgDuration * static_cast<double>(nThreads);
            if (minDuration < event.minDuration)
                event.minDuration = minDuration;
            if (maxDuration > event.maxDuration)
                event.maxDuration = maxDuration;
            event.avgDuration += sumDuration;
            event.nThreads += nThreads;      
        }
    }

    // averaging events
    for (auto& [event_name, event] : event_map)
    {
        if (event.nThreads > 0)
            event.avgDuration /= static_cast<double>(event.nThreads);
    }

    return timer;
}


template<typename T>
std::string profiler::center(const T& val_, int width)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3) << val_;
    std::string s = ss.str();
    if ((int)s.size() >= width)
        return s;

    int left = (width - s.size()) / 2;
    int right = width - s.size() - left;

    return std::string(left, ' ')
        + s
        + std::string(right, ' ');
}

