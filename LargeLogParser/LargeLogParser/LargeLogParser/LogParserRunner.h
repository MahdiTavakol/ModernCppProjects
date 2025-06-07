#pragma once


#include "LogParser.h"
#include "LogParserParallel.h"

#include <future>
#include <string>
#include <vector>

enum class ReturnMode
{
	COPY,
	MOVE
};

class LogParserRunner
{
	using ARRAY_INT_3 = std::array<int, 3>;
	using ARRAY_DATA_3 = std::array<std::vector<std::string>, 3>;

public:
	LogParserRunner(std::string filePath_, const int& numThreads_);
	void parseLogs();
	template<ReturnMode returnMode>
	void returnLogs(ARRAY_DATA_3& tot_data_);

private:
	std::string filePath;
	int fileLength;

	int threadId, numThreads;

	std::vector<std::thread> threads;
	std::vector<LogParserParallel> logParsers;

	
	ARRAY_INT_3 tot_nums;
	ARRAY_DATA_3 tot_data;
};