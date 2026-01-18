#include "LogParserFuture.h"
#include "DataStructure.h"
#include <filesystem>


void LogParserFuture::readFile()
{
	std::vector<std::future<DataStructure>> ftrs;
	ftrs.reserve(num_threads);

	for (int i = 0; i < num_threads; i++)
	{
		ftrs.push_back(std::async(&LogParserFuture::readChunk,this, i, num_threads));
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
	beg = me_ * lengthPerThread;
	end = beg + lengthPerThread;
	if (end > fileLength) end = fileLength;

	std::unique_ptr<LogParser> logParser =
		std::make_unique<LogParser>(this->filePath, this->fileLength, this->beg, this->end);

	logParser->readFile();
	logParser->returnLogs<ReturnMode::MOVE>(output_data);

	return output_data;

}