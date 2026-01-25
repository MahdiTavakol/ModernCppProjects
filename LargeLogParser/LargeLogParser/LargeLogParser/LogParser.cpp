#include "LogParser.h"
#include <filesystem>


LogParser::LogParser(std::string filePath_, std::array<std::string, 3> keys) :
	filePath{ filePath_ }, beg{ 0 }, end{ 0 },
	error_key{ keys[0] }, warn_key{ keys[1] }, info_key{ keys[2] }
{
	file.open(filePath);
	if (!file.is_open())
		throw std::runtime_error("Cannot open the file " + filePath + " for reading!");
	fileLength = this->inquireFileLength(filePath_);
	end = fileLength;
}

LogParser::LogParser(std::string filePath_, const int& file_length_, int beg_, int end_) :
	LogParser{ filePath_ }
{
	fileLength = file_length_;
	beg = beg_;
	end = end_;
}


void LogParser::readFile()
{
	file.seekg(beg);
	std::string line;

	std::vector<std::string>& errors = data_struct.returnData()[0];
	std::vector<std::string>& warns = data_struct.returnData()[1];
	std::vector<std::string>& infos = data_struct.returnData()[2];

	int& num_lines   = data_struct.returnNumLines();
	int& num_infos   = data_struct.returnNums()[0];
	int& num_warns   = data_struct.returnNums()[1];
	int& num_errors  = data_struct.returnNums()[2];

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
		num_lines++;
		if (line.find(error_key) != std::string::npos) {
			errors.push_back(line);
			num_errors++;
		} else if (line.find(warn_key) != std::string::npos) {
			warns.push_back(line);
			num_warns++;
		} else if (line.find(info_key) != std::string::npos){
			infos.push_back(line);
			num_infos++;
		}

		int pos = static_cast<int>(file.tellg());
		if (pos >= end)
			break;

		double progress = 100 * (pos - beg) / (end - beg);

		printProgress(progress);
	}
}

void LogParser::printProgress(const double& progress)
{
	//std::cout << std::format("Progress {:.2f}%", progress) << std::endl;
}


int LogParser::inquireFileLength(std::string filePath_)
{
	std::filesystem::path p = filePath_;
	if (!std::filesystem::exists(p)) {
		throw std::runtime_error("File does not exist: " + filePath_);
	}
	return static_cast<int>(std::filesystem::file_size(p));
}