#pragma once
#include <map>
#include <chrono>
#include <unordered_map>
#include <mutex>
#include "Logger.h"
#include "../Algorithms/communicator.h"
using Duration = std::chrono::duration<double, std::milli>;
using Time = std::chrono::steady_clock::time_point;

struct TimingInfo {
	Time myStart;
	Duration myDuration;
	std::vector<Duration> children_durations;
	// info obtained from this object and children
	Duration minDuration, avgDuration, maxDuration;
	// number of threads used for each event
	int nThreads;
	// number of profilings done.
	int nProfilings;

	TimingInfo():
		myStart{},
		myDuration{0},
		minDuration{0},
		avgDuration{0},
		maxDuration{0},
		nThreads{1},
		nProfilings{0}
	{}

	TimingInfo(Duration myMinDuration_, 
		       Duration myAvgDuration_,
		       Duration myMaxDuration_,
		       int myThreads_):
		myStart{},
		myDuration{ 0 },
		minDuration{ myMinDuration_ },
		avgDuration{ myAvgDuration_ },
		maxDuration{ myMaxDuration_ },
		nThreads{ myThreads_ },
		nProfilings{0}
	{}
		

	TimingInfo(Time myStart_):
		myStart{myStart_},
		myDuration{0},
		minDuration{0},
		avgDuration{0},
		maxDuration{0},
		nThreads{1},
		nProfilings{1}
	{}
};

class profiler
{
public:
	profiler(communicator* para_, Logger* error_);
	void reset_program_timer();
	void start_event(const std::string event_name_);
	void stop_event(const std::string event_name_);
	// since these functions are called by a thread I would use passing by copy rather than through reference
	std::string start_thread_event(const std::string event_name_);
	void start_thread_event(const std::string thread_name_, const std::string event_name_);
	void stop_thread_event(const std::string thread_name_, const std::string event_name_);


	void return_event_duration(const std::string event_name_, std::vector<Duration>& durations_);
	void add_child(profiler* child_);
	void update_average_info();
	void print_timing_info();

	static std::unique_ptr<profiler> average_multiple_profilers(std::vector<std::unique_ptr<profiler>>& profiler_vec_);



protected:
	std::string profiler_name;
	// error and parallel communicator pointers
	communicator* para;
	Logger* error;
	// children profilers --->>> for threads
	std::unordered_map<std::string, std::unique_ptr<profiler>> thread_profilers;
	// when the program did start
	Time program_start;
	// timing info for various events
	std::unordered_map<std::string, TimingInfo> timing_infos;
	//mutex for threads to safely add to the thread_profilers
	std::mutex mtx;
	// the duration of the progrma
	Duration program_duration_avg, program_duration_min, program_duration_max;
};