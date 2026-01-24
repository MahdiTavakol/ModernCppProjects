#pragma once

#include "fileProp.h"

#include <string>
#include <fstream>
#include <filesystem>

class CreateLargeLogs
{
public:
	CreateLargeLogs(const std::string& logFileName_, const int& num_lines_, const int& line_length_,
		const double& info_percent_, const double& warns_percent_, const double& errors_percent_,
		const bool& keep_temp_ = false):
		logFile{ logFileName_ }, logFileName{logFileName_}, num_lines{ num_lines_ }, line_length{ line_length_ },
		info_percent{ info_percent_ }, warns_percent{ warns_percent_ }, errors_percent{ errors_percent_ },
		num_infos{ 0 }, num_warns{ 0 }, num_errors{ 0 }, keep_temp{keep_temp_}
	{
		if (info_percent_ + warns_percent_ + errors_percent_ > 100.0)
			throw std::invalid_argument("The total percent is larger than 100%!");
	}

	~CreateLargeLogs()
	{
		if (!keep_temp)
		{
			/* 
			 * If we want to delete the file we have to close it,
			 * otherwise the C++ itself closes the file after an
			 * object of this class goes out of the scope.
			 */
			logFile.close();
			std::filesystem::remove(logFileName);
		}
	}

	void generateLog();
	int operator()(int& num_infos_, int& num_warns_, int& num_errors_);


private:
	// file information
	int num_lines, line_length;
	// file contents
	int num_infos, num_warns, num_errors;
	double info_percent, warns_percent, errors_percent;


	std::ofstream logFile;
	std::string logFileName;
	int num_infos, num_warns, num_errors;
	bool keep_temp = false;

};
