#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <array>
#include <ios>
#include <iostream>
#include <format>
#include <mutex>

class LogParser
{
public:
	LogParser(std::string filePath_, std::array<std::string,3> keys = { "[ERROR]", "[WARN]", "[INFO]" });
	LogParser(std::string filePath_, const int& file_length_, int beg_, int end_);
	virtual void readFile();
	virtual void printProgress(const double& progress);
	void returnNumErrorWarnInfo(std::array<int,3>& nums);
	void returnErrorWarnInfo(std::array<std::vector<std::string>, 3>& ouput);
	int returnNumLines();
	void returnFileLength(int& fileLength_) { fileLength_ = fileLength; }
	int operator()(int& num_infos_, int& num_warns_, int& num_errors_);

	static int inquireFileLength(std::string filePath_);

protected:
	std::string filePath;
	std::ifstream file;
	int fileLength, beg, end;

	const std::string error_key, warn_key, info_key;
	int num_lines, num_errors, num_warns, num_infos;
	std::vector<std::string> errors, warns, infos;
};
