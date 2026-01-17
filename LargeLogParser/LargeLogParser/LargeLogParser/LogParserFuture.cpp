#include "LogParserFuture.h"
#include "shared.h"
#include <filesystem>

void LogParserFuture::initialize()
{
	std::filesystem::path p = filePath;
	if (!std::filesystem::exists(p)) {
		throw std::runtime_error("File does not exist: " + filePath);
	}
	fileLength = std::filesystem::file_size(p);

	if (!file.is_open())
		throw std::runtime_error("Cannot open the file " + filePath + " for reading!");
}

void LogParserFuture::readFile()
{
	std::vector<std::future<DATA_STRUCT>> ftrs;
	ftrs.reserve(num_threads);

	for (int i = 0; i < num_threads; i++)
	{
		ftrs.push_back(std::async(&LogParserFuture::readChunk,this, i, num_threads));
	}

	for (auto& ftr : ftrs)
	{
		DATA_STRUCT out = ftr.get();


		errors.insert(errors.end(),out.data[0].begin(), out.data[0].end());
		warns.insert(warns.end(), out.data[1].begin(), out.data[1].end());
		infos.insert(infos.end(), out.data[2].begin(), out.data[2].end());

		num_errors += out.nums[0];
		num_warns += out.nums[1];
		num_infos += out.nums[2];
	}
}

DATA_STRUCT LogParserFuture::readChunk(const int& me_, const int& num_)
{
	DATA_STRUCT output_data;

	int lengthPerThread = static_cast<int>(fileLength / num_);
	beg = me_ * lengthPerThread;
	end = beg + lengthPerThread;
	if (end > fileLength) end = fileLength;


	std::fstream file{ filePath };

	file.seekg(beg);
	std::string line;

	/* It is possible the[ERROR] is in the middle of the line
	*  something like timestap -> [ERROR] and this thread
	*  start from ->. So this and the previous threads
	*  read this line we need to discard the line
	*  if we start from the middle since the previous
	*  thread had scanned the whole line.
	*/
	if (beg != 0) {
		file.seekg(beg - 1);
		char prevChar;
		file.get(prevChar);
		// We are in the middle of a line so discard whatever left of the line
		if (prevChar != '\n')
		{
			std::getline(file, line);
		}
	}

	while (getline(file, line))
	{
		if (line.find(error_key) != std::string::npos) {
			output_data.data[0].push_back(line);
			output_data.nums[0]++;
		}
		else if (line.find(warn_key) != std::string::npos) {
			output_data.data[1].push_back(line);
			output_data.nums[1]++;
		}
		else if (line.find(info_key) != std::string::npos) {
			output_data.data[2].push_back(line);
			output_data.nums[2]++;
		}

		int pos = file.tellg();
		if (pos >= end)
			break;

		double progress = 100 * (pos - beg) / (end - beg);

		printProgress(progress);
	}

	return output_data;

}