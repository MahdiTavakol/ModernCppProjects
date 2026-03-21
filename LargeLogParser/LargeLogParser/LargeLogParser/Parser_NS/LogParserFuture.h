#pragma once

#include <vector>
#include <future>

#include "../Data_NS/DataStructure.h"
#include "LogParser.h"

namespace Parser_NS {
	class LogParserFuture final : public LogParser
	{
	public:
		LogParserFuture(std::string filePath_, int num_threads_, bool silentMode_) :
			LogParser{ filePath_ , silentMode_}
		{
			num_threads = num_threads_;
		}
		~LogParserFuture() = default;

		virtual void readFile() override;


	private:
		int num_threads;
		DataStructure readChunk(const int& me_, const int& num_, bool silentMode_ = true);
	};
} // namespace Parser_NS