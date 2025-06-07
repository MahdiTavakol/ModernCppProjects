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

enum class ExecMode
{
	SERIAL,
	PARALLEL
};

class LogParser
{
public:
	LogParser(std::string filePath_, std::array<std::string,3> keys, ExecMode execMode_ = ExecMode::SERIAL);
	LogParser(std::string filePath_, ExecMode execMode_ = ExecMode::SERIAL) {
		LogParser(filePath_, std::array<std::string, 3>{"[ERROR]", "[WARN]", "[INFO]"}, execMode_);
	}
	void readFile();
	void returnNumErrorWarnInfo(std::array<int,3>& nums);
	void returnErrorWarnInfo(std::array<std::vector<std::string>, 3>& ouput);
	void returnFileLength(int& fileLength_) { fileLength_ = fileLength; }

protected:
	ExecMode execMode;

	int thread_id, num_threads;

	std::string filePath;
	std::ifstream file;
	int fileLength, beg, end;

	const std::string error_key, warn_key, info_key;
	int num_errors, num_warns, num_infos;
	std::vector<std::string> errors, warns, infos;
};
