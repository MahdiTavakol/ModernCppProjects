#pragma once


#include "shared.h"
#include "LogParser.h"

#include <future>
#include <string>
#include <vector>
#include <format>

enum class ReturnMode
{
	COPY,
	MOVE
};


class LogParserThreads: public LogParser
{

public:
	LogParserThreads(std::string filePath_, const int& numThreads_);
	void parseLogs();
	template<ReturnMode returnMode>
	void returnLogs(ARRAY_DATA_3& tot_data_);
	int operator()(int& num_infos_, int& num_warns_, int& num_errors_);

	void printProgress(const double& progress) override
	{
		int id = 0;
		for (auto& logParser : logParsers) {
			std::lock_guard<std::mutex> lock(cout_mutex);
			std::cout << std::format("[Thread {}] Progress: {:.2f}%", id++, progress) << std::endl;
		}
	}

private:
	int numThreads;
	std::mutex cout_mutex;

	std::vector<std::thread> threads;
	std::vector<LogParser> logParsers;

	
	ARRAY_INT_3 tot_nums;
	ARRAY_DATA_3 tot_data;

	int num_lines;
};