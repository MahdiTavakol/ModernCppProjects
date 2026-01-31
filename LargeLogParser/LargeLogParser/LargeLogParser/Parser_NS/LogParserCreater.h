#pragma once
#include <vector>
#include <string>
#include <memory>

#include "LogParser.h"
#include "LogParserThreads.h"
#include "LogParserFuture.h"

namespace Parser_NS {
	
	class LogParserCreater {
	public:
		LogParserCreater(const std::vector<std::string>& args_) {}
		LogParserCreater() = default;
		virtual std::unique_ptr<LogParser>&& operator()() {
			return std::move(logParserPtr);
		}

	protected:
		// The logParser pointer 
		std::unique_ptr<LogParser> logParserPtr;
	};


}
