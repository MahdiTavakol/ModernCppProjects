#pragma once
#include <map>
#include <chrono>
#include "Logger.h"
#include "../Algorithms/communicator.h"
using Duration = std::chrono::duration<double, std::milli>;

class profiler
{
public:
	profiler(communicator* para_, Logger* error_);

	void start_event(const std::string event_name_);
	void stop_event(const std::string event_name_);
	void print_timing_info() const;

protected:
	communicator* para;
	Logger* error;
	void set_mapping();
	std::map<std::string, Duration> timings;
};