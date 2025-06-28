#include "LogParserParallelRunner.h"

#include <filesystem>

LogParserParallelRunner::LogParserParallelRunner(std::string filePath_, const int& numThreads_) :
	filePath{ filePath_ }, numThreads{ numThreads }, tot_nums{ {0,0,0} }, num_lines{0}
{
	std::filesystem::path p = filePath;
	if (!std::filesystem::exists(p)) {
		throw std::runtime_error("File does not exist: " + filePath);
	}
	fileLength = std::filesystem::file_size(p);


	logParsers.reserve(numThreads);
	threads.reserve(numThreads);
}

void LogParserParallelRunner::parseLogs()
{
	for (int i = 0; i < numThreads; i++)
	{
		int threadId = i;
		logParsers.emplace_back(filePath, fileLength, threadId, numThreads);
		logParsers[i].initialize();
		threads.emplace_back(&LogParserParallel::readFile, &logParsers[i]);
	}

	// waiting for all the threads to finish execution
	for (auto& t : threads)
		t.join();

	for (auto& parser : logParsers)
	{
		ARRAY_INT_3 nums;
		ARRAY_DATA_3 data;
		parser.returnNumErrorWarnInfo(nums);
		parser.returnErrorWarnInfo(data);
		num_lines += parser.returnNumLines();

		for (int i = 0; i < 3; i++)
		{
			tot_nums[i] += nums[i];
			tot_data[i].insert(tot_data[i].end(), data[i].begin(), data[i].end());
		}
	}
}

template<ReturnMode returnMode>
void LogParserParallelRunner::returnLogs(ARRAY_DATA_3& tot_data_)
{
	switch (returnMode)
	{
		case ReturnMode::COPY:
			tot_data_ = tot_data;
			return;
		case ReturnMode::MOVE:
			tot_data_ = std::move(tot_data);
			return;
		default:
			throw std::invalid_argument("Wrong return mode!");
	}
}

int LogParserParallelRunner::operator()(int& num_infos_, int& num_warns_, int& num_errors_)
{
	num_errors_ = tot_nums[0];
	num_warns_ = tot_nums[1];
	num_infos_ = tot_nums[2];
	return num_lines;
}