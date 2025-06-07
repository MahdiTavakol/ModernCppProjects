#include "LogParserRunner.h"

LogParserRunner::LogParserRunner(std::string filePath_, const int& numThreads_) :
	filePath{ filePath_}, numThreads{ numThreads }
{
	std::unique_ptr<LogParser> parserSerialPtr = std::make_unique<LogParser>( filePath );
	parserSerialPtr->returnFileLength(fileLength);
	logParsers.reserve(numThreads);
	threads.reserve(numThreads);
}

void LogParserRunner::parseLogs()
{
	for (int i = 0; i < numThreads; i++)
	{
		threadId = i;
		logParsers.emplace_back(filePath, fileLength, threadId, numThreads);
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

		for (int i = 0; i < 3; i++)
		{
			tot_nums[i] += nums[i];
			tot_data[i].insert(tot_data[i].end(), data[i].begin(), data[i].end());
		}
	}
}

template<ReturnMode returnMode>
void LogParserRunner::returnLogs(ARRAY_DATA_3& tot_data_)
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