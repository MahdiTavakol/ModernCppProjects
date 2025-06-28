#pragma once

#include <vector>
#include <future>

#include "LogParser.h"

class LogParserParallelFuture : public LogParser
{
	using ARRAY_INT_3 = std::array<int, 3>;
	using ARRAY_DATA_3 = std::array<std::vector<std::string>, 3>;
	using DATA_STRUCT =
	struct {
		ARRAY_INT_3 nums;
		ARRAY_DATA_3 data;
	};

public:
	LogParserParallelFuture(std::string filePath_, int num_threads_) :
		LogParser{ filePath_ }
	{
		thread_id = -1;
		num_threads = num_threads_;
	}

	virtual void initialize() override;
	virtual void readFile() override;
	

private:
	DATA_STRUCT readChunk(const int& me_, const int& num_);
};