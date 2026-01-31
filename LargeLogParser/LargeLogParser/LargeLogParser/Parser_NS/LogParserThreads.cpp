#include "LogParserThreads.h"


using namespace Parser_NS;

LogParserThreads::LogParserThreads(std::string filePath_, const int& numThreads_, bool silentMode_) :
	LogParser{ filePath_, silentMode_}, numThreads{ numThreads_ }
{
	logParsers.reserve(numThreads);
	threads.reserve(numThreads);

	// Initializing the file logParsers
	/* Note: The logParser initializer has set the fileLength */

	for (int i = 0; i < numThreads; i++) {
		int lengthPerThread = (fileLength + numThreads - 1) / numThreads;
		int beg = i * lengthPerThread;
		int end = beg + lengthPerThread;
		end = end >= fileLength ? fileLength : end;
		logParsers.emplace_back(filePath, fileLength, beg, end);
	}
}

LogParserThreads::LogParserThreads(std::string filePath_, const int& numThreads_,
	const int& beg_, const int& end_, bool silentMode_) :
	LogParser{ filePath_, silentMode_ }, numThreads{ numThreads_ }
{
	logParsers.reserve(numThreads);
	threads.reserve(numThreads);

	fileLength = end_ - beg_;

	// Initializing the file logParsers
	/* Note: The logParser initializer has set the fileLength */

	for (int i = 0; i < numThreads; i++) {
		int lengthPerThread = (fileLength + numThreads - 1) / numThreads;
		int beg = beg_ + i * lengthPerThread;
		int end = beg + lengthPerThread;
		end = end >= end_ ? fileLength : end_;
		if (i == 0)
			logParsers.emplace_back(filePath, fileLength, beg, end, silentMode);
		else
			logParsers.emplace_back(filePath, fileLength, beg, end, false);
	}
}

void LogParserThreads::readFile()
{
	for (int i = 0; i < numThreads; i++)
	{
		threads.emplace_back(&LogParser::readFile, &logParsers[i]);
	}

	// waiting for all the threads to finish execution
	for (auto& t : threads)
		t.join();

	for (auto& parser : logParsers)
	{
		DataStructure newData;
		parser.returnLogs<ReturnMode::MOVE>(newData);
		this->data_struct.append(std::move(newData));
	}
}
