#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <functional>

enum class print_mode
{
	DEBUG,
	RELEASE,
	PROFILING
};



class Logger {
public:
	Logger();

	Logger(const std::vector<std::reference_wrapper<std::ostream>>& strms_);

	void reset_mode(print_mode mode_);
	void print_message(const std::string text) const;
	void print_message(const std::string message_, int level_ = 0, char delimiter = ' ') const;
	void print_message(std::iostream& input_strm_) const;
	void print_error(const std::string message_) const;

private:
	std::vector<std::reference_wrapper<std::ostream>> strms;
	int max_message_level = 1000;
	int max_debug_message_level = 1;
	print_mode mode = print_mode::RELEASE; //print_mode::DEBUG;

	const int Debug_level = -1;
	const int Profiler_level = -2;

};