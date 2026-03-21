#pragma once

#include "LogParserCreater.h"
#include "LogParserCreaterConc.h"

namespace Parser_NS {
	class LogParserCreaterLazy final : public LogParserCreater {
	public:
		LogParserCreaterLazy(const std::vector<std::string>& args_);
		std::unique_ptr<LogParser>&& operator()() override;

	private:
		std::vector<std::string> args;
	};
} // namespace Parser_NS
