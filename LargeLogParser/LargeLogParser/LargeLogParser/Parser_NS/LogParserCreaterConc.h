#pragma once

#include "LogParserCreater.h"

namespace Parser_NS {

	class LogParserCreaterConc final : public LogParserCreater {
		public:
		LogParserCreaterConc(const std::vector<std::string>& args_);
	};
} // namespace Parser_NS

