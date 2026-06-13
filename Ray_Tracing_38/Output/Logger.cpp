#include "Logger.h"

Logger::Logger():
	strms{std::ref(std::cout)}
{}

Logger::Logger(const std::vector<std::reference_wrapper<std::ostream>>& strms_) :
	strms{ strms_ }
{
}

void Logger::print_message(const std::string& text) const
{
	for (auto& strm : strms)
		strm.get() << text << std::endl;
}

void Logger::print_message(const std::string& message_, int level_, char delimiter_) const
{
	std::string msg;

	if ((mode == print_mode::DEBUG &&
		level_ > max_debug_message_level) ||
		level_ > max_message_level)
		return;

	if (mode == print_mode::DEBUG)
	{
		if (level_ == Debug_level)
		{
			msg = std::string(max_debug_message_level + 1, delimiter_) + message_;
		}
	}
	else
	{
		level_ = level_ < 0 ? 0 : level_;
		msg = std::string(level_, delimiter_) + message_;
	}
	

	for (auto& strm : strms)
		strm.get() << msg << std::endl;
}

void Logger::print_error(const std::string& message_) const
{
	//for (auto& strm : strms)
	//	strm.get() << message_ << std::endl;
	std::cerr << message_ << std::endl;
}