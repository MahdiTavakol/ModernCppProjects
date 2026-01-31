#include "LogParserCreaterLazy.h"

using namespace Parser_NS;

LogParserCreaterLazy::LogParserCreaterLazy(const std::vector<std::string>& args_) :
	args{ args_ }
{}

std::unique_ptr<LogParser>&& LogParserCreaterLazy::operator()()
{
	LogParserCreaterConc parserCreaterConc{ args };
	logParserPtr = std::move(parserCreaterConc());
	return std::move(logParserPtr);
}