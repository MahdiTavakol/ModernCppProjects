#pragma once

#include <vector>
#include <future>

#include "shared.h"
#include "LogParser.h"

class LogParserFuture : public LogParser
{


public:
	LogParserFuture(std::string filePath_, int num_threads_) :
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