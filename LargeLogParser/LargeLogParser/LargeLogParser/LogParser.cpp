#include "LogParser.h"
#include <filesystem>


LogParser::LogParser(std::string filePath_, std::array<std::string, 3> keys) :
	filePath{ filePath_ }, beg{ 0 }, end{ 0 },
	num_lines{0}, num_errors{ 0 }, num_warns{ 0 }, num_infos{ 0 },
	error_key{ keys[0] }, warn_key{ keys[1] }, info_key{ keys[2] }
{
	file.open(filePath);
	if (!file.is_open())
		throw std::runtime_error("Cannot open the file " + filePath + " for reading!");
}

void LogParser::initialize()
{
	std::filesystem::path p = filePath;
	if (!std::filesystem::exists(p)) {
		throw std::runtime_error("File does not exist: " + filePath);
	}
	fileLength = std::filesystem::file_size(p);

	thread_id = 0;
	num_threads = 1;

	file.seekg(0, file.beg);
}

void LogParser::readFile()
{
	initialize();

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

		int pos = file.tellg();
		if (pos >= end)
			break;

		double progress = 100 * (pos - beg) / (end - beg);

		printProgress(progress);
	}
}

void LogParser::printProgress(const double& progress)
{
	std::cout << std::format("Progress {:.2f}%", progress) << std::endl;
}

void LogParser::returnNumErrorWarnInfo(std::array<int, 3>& nums_)
{
	nums_[0] = num_errors;
	nums_[1] = num_warns;
	nums_[2] = num_infos;
}

int LogParser::returnNumLines()
{
	return num_lines;
}

void LogParser::returnErrorWarnInfo(std::array<std::vector<std::string>, 3>& output_)
{
	if (errors.empty())
	{
		std::cout << "Warning the errors have already been returned, reading the file again!" << std::endl;
		readFile();
	}
	output_[0] = std::move(errors);
	output_[1] = std::move(warns);
	output_[2] = std::move(infos);
	num_errors = 0;
	num_warns = 0;
	num_infos = 0;
}

int LogParser::operator()(int& num_infos_, int& num_warns_, int& num_errors_)
{
	num_infos_ = num_infos;
	num_warns_ = num_warns;
	num_errors_ = num_errors;
	return num_lines;
}