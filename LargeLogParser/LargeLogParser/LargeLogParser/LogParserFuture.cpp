#include "LogParserFuture.h"
#include "DataStructure.h"
#include <filesystem>


void LogParserFuture::readFile()
{
	std::vector<std::future<DataStructure>> ftrs;
	ftrs.reserve(num_threads);

	for (int i = 0; i < num_threads; i++)
	{
		ftrs.push_back(std::async(std::launch::async, & LogParserFuture::readChunk, this, i, num_threads));
	}

	for (auto& ftr : ftrs)
	{
		DataStructure newData = ftr.get();
		this->data_struct.append(std::move(newData));
	}
}

DataStructure LogParserFuture::readChunk(const int& me_, const int& num_)
{
	DataStructure output_data;

	int lengthPerThread = static_cast<int>(fileLength / num_);
	int mybeg = me_ * lengthPerThread;
	int myend = mybeg + lengthPerThread;
	if (myend > fileLength) myend = fileLength;

	std::unique_ptr<LogParser> logParser =
		std::make_unique<LogParser>(this->filePath, this->fileLength, mybeg, myend);

	logParser->readFile();
	logParser->returnLogs<ReturnMode::MOVE>(output_data);

	return output_data;

}