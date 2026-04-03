#include "Logger.h"

void Logger::print(std::string& text) const
{
	for (auto& strm : strms)
		strm.get() << text << std::endl;
}