#include "Logger.h"

void Logger::print(const std::string& text) const
{
	for (auto& strm : strms)
		strm.get() << text << std::endl;
}