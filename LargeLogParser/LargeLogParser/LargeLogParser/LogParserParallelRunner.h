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


class LogParserParallelRunner
{
	using ARRAY_INT_3 = std::array<int, 3>;
	using ARRAY_DATA_3 = std::array<std::vector<std::string>, 3>;

public:
	LogParserParallelRunner(std::string filePath_, const int& numThreads_);
	void parseLogs();
	template<ReturnMode returnMode>
	void returnLogs(ARRAY_DATA_3& tot_data_);
	int operator()(int& num_infos_, int& num_warns_, int& num_errors_);

private:
	std::string filePath;
	int fileLength;

	int numThreads;

	std::vector<std::thread> threads;
	std::vector<LogParserParallel> logParsers;

	
	ARRAY_INT_3 tot_nums;
	ARRAY_DATA_3 tot_data;

	int num_lines;
};