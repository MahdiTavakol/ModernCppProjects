#include "Logger.h"
#include <sstream>

Logger::Logger():
	strms{std::ref(std::cout)}
{}

Logger::Logger(const std::vector<std::reference_wrapper<std::ostream>>& strms_) :
	strms{ strms_ }
{
}

void Logger::reset_mode(print_mode mode_)
{
	mode = mode_;
}

void Logger::print_message(const std::string text) const
{
	for (auto& strm : strms)
		strm.get() << text << std::endl;
}

void Logger::print_message(const std::string message_, int level_, char delimiter_) const
{
	std::string msg;

	bool escape_1 = mode == print_mode::DEBUG && level_ > max_debug_message_level;
	bool escape_2 = mode == print_mode::RELEASE && level_ > max_message_level;
	bool escape_3 = mode == print_mode::PROFILING && level_ != Profiler_level;

	if (escape_1 || escape_2 || escape_3)
		return;

	if (mode == print_mode::RELEASE)
	{
		level_ = level_ < 0 ? 0 : level_;
		msg = std::string(level_, delimiter_) + message_;
	}
	else if (mode == print_mode::PROFILING)
	{
		if (level_ == Profiler_level)
		{
			msg = message_;
		}
	}
	else if (mode == print_mode::DEBUG)
	{
		if (level_ == Debug_level)
		{
			msg = std::string(max_debug_message_level + 1, delimiter_) + message_;
		}
	}

	

	for (auto& strm : strms)
		strm.get() << msg << std::endl;
}

void Logger::print_error(const std::string message_) const
{
	//for (auto& strm : strms)
	//	strm.get() << message_ << std::endl;
	std::cerr << message_ << std::endl;
}

void Logger::print_message(std::iostream& input_strm_) const
{
	constexpr int max_level = 5;
	std::string line;

	std::vector<std::stringstream> sorted_strm;
	sorted_strm.resize(max_level);

	while (std::getline(input_strm_, line))
	{
		size_t nspaces = line.find_first_not_of(' ');
		if (nspaces < max_level)
		{
			sorted_strm[nspaces] << line << std::endl;
		}
	}

	for (int i = 0; i < max_level; i++)
	{
		while (std::getline(sorted_strm[i], line))
		{
			for (auto& strm : strms)
			{
				strm.get() << line << std::endl;
			}
		}
	}


	/*
	// printing the stream line by line
	while (std::getline(input_strm_, line))
	{
		for (auto& strm : strms)
		{
			strm.get() << line << std::endl;
		}
	}*/
}