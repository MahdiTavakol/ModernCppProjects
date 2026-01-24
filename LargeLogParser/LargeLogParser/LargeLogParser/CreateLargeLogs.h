#pragma once

#include "fileWriter.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <memory>

class CreateLargeLogs
{
public:
	CreateLargeLogs(const std::string& logFileName_, const int& num_lines_, const int& line_length_,
		const double& info_percent_, const double& warns_percent_, const double& errors_percent_,
		const bool& keep_temp_ = false);

	CreateLargeLogs(std::unique_ptr<fileWriter>&& fileWriterPtr_) :
	    fileWriterPtr{std::move(fileWriterPtr_)} {}

	~CreateLargeLogs();

	void generateLog();
	int operator()(std::array<int,3>& nums_);


private:
    // file 
	std::unique_ptr<fileWriter> fileWriterPtr;

	bool keep_temp = false;

};
