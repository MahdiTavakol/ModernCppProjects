#include "LogParser.h"
#include <filesystem>
#include <string>

using namespace Parser_NS;

LogParser::LogParser(std::string filePath_, bool silentMode_, std::array<std::string, 3> keys) :
	filePath{ filePath_ }, silentMode{silentMode_},
	beg{ 0 }, end{ 0 },
	error_key{ keys[0] }, warn_key{ keys[1] }, info_key{ keys[2] }
{
	file.open(filePath);
	if (!file.is_open())
		throw std::runtime_error("Cannot open the file " + filePath + " for reading!");
	fileLength = this->inquireFileLength(filePath_);
	end = fileLength;
}

LogParser::LogParser(std::string filePath_, const int& file_length_, 
	int beg_, int end_, bool silentMode_) :
	LogParser{ filePath_ , silentMode_}
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

		if (!silentMode)
			printProgress(progress);
	}
}

void LogParser::printProgress(const double& progress)
{
	std::cout << std::format("Progress {:.2f}%", progress) << std::endl;
}

void LogParser::writeData()
{
	std::string fileNameOnly;
	std::stringstream iss(filePath);
	std::getline(iss, fileNameOnly, ','); // to get the file name only without the extension

	auto writeStream = [](const std::string& fileName_, 
		                  const std::vector<std::string>& data_) {
		std::ofstream stream(fileName_);
		for (const auto& line : data_)
			stream << line << std::endl;
		stream.close();
		};
	
	struct {
		std::string fileName_;
		std::vector<std::string> data_;
	} output[3] = {
		{ fileNameOnly + "-infos.log", data_struct.returnData()[0] },
		{ fileNameOnly + "-warns.log", data_struct.returnData()[1] },
		{ fileNameOnly + "-errors.log", data_struct.returnData()[2] }
	};

	for (const auto& out : output)
		writeStream(out.fileName_, out.data_);
}

int LogParser::inquireFileLength(std::string filePath_)
{
	std::filesystem::path p = filePath_;
	if (!std::filesystem::exists(p)) {
		throw std::runtime_error("File does not exist: " + filePath_);
	}
	return static_cast<int>(std::filesystem::file_size(p));
}