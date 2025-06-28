#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include "LogParser.h"

class LogParserParallel: public LogParser
{
public:
	LogParserParallel(std::string filePath_, const int& file_length_, int thread_id_, int num_threads_):
		LogParser{ filePath_}
	{
		fileLength = file_length_;
		thread_id = thread_id_;
		num_threads = num_threads_;
	}

	void initialize() override {
		int lengthPerThread = static_cast<int>(fileLength / num_threads);
		beg = thread_id * lengthPerThread;
		end = beg + lengthPerThread;
		if (end > fileLength) end = fileLength;
	}

	void printProgress(const double& progress) override
	{
		std::lock_guard<std::mutex> lock(cout_mutex);
		std::cout << std::format("[Thread {}] Progress: {:.2f}%", thread_id, progress) << std::endl;
	}

private:
	std::mutex cout_mutex;
};
