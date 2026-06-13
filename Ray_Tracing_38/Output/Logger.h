#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <functional>

enum class print_mode
{
	DEBUG,
	RELEASE
};

constexpr int Debug_level = -1;

class Logger {
public:
	Logger();

	Logger(const std::vector<std::reference_wrapper<std::ostream>>& strms_);

	void print_message(const std::string& text) const;
	void print_message(const std::string& message_, int level_ = 0, char delimiter = ' ') const;
	void print_error(const std::string& message_) const;

private:
	std::vector<std::reference_wrapper<std::ostream>> strms;
	int max_message_level = 1000;
	int max_debug_message_level = 1;
	print_mode mode = print_mode::RELEASE; //print_mode::DEBUG;

};