#pragma once

#include <vector>
#include <future>

#include "DataStructure.h"
#include "LogParser.h"

class LogParserFuture final : public LogParser
{


public:
	LogParserFuture(std::string filePath_, int num_threads_) :
		LogParser{ filePath_ }
	{
		num_threads = num_threads_;
	}

	virtual void readFile() override;
	

private:
	int num_threads;
	DataStructure readChunk(const int& me_, const int& num_);
};