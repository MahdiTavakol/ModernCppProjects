#include "LogParserRunner.h"

#include <filesystem>

LogParserRunner::LogParserRunner(std::string filePath_, const int& numThreads_) :
	filePath{ filePath_ }, numThreads{ numThreads }, tot_nums{ {0,0,0} }
{
	/*
	* I do not want each instance of parallel log parser read the file size
	* so I on purpose created a serial log parser just to get the file size.
	*/
	//std::unique_ptr<LogParser> parserSerialPtr = std::make_unique<LogParser>( filePath );
	//parserSerialPtr->returnFileLength(fileLength);

	std::filesystem::path p = filePath;
	if (!std::filesystem::exists(p)) {
		throw std::runtime_error("File does not exist: " + filePath);
	}
	fileLength = std::filesystem::file_size(p);


	logParsers.reserve(numThreads);
	threads.reserve(numThreads);
}

void LogParserRunner::parseLogs()
{
	for (int i = 0; i < numThreads; i++)
	{
		int threadId = i;
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