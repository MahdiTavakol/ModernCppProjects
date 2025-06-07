#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include "LogParser.h"

class LogParserParallel: public LogParser
{
public:
	LogParserParallel(std::string filePath_, int fileLength_, int thread_id_, int num_threads_):
		LogParser{ filePath_,ExecMode::PARALLEL }
	{
		thread_id = thread_id_;
		num_threads = num_threads_;
		fileLength = fileLength_;
		int lengthPerThread = static_cast<int>(fileLength_ / num_threads_);
		beg = thread_id * lengthPerThread;
		end = beg + lengthPerThread;
		if (end > fileLength_) end = fileLength;
	}
};
