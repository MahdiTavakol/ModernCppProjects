#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <format>
#include <array>
#include <ios>
#include <iostream>
#include <format>
#include <mutex>
#include "DataStructure.h"

enum class ReturnMode
{
	COPY,
	MOVE
};

class LogParser
{
public:
	LogParser(std::string filePath_, std::array<std::string,3> keys = { "[ERROR]", "[WARN]", "[INFO]" });
	LogParser(std::string filePath_, const int& file_length_, int beg_, int end_);
	virtual void readFile();
	/*void readFile(int& beg_, int& end_) {
		this->beg = beg_; this->end = end_;
		readFile();
	}*/
	virtual void printProgress(const double& progress);
	template<ReturnMode returnMode>
	void returnLogs(DataStructure& data_struct_);
	void returnFileLength(int& fileLength_) { fileLength_ = fileLength; }


	static int inquireFileLength(std::string filePath_);

protected:
	// file info
	std::string filePath;
	std::ifstream file;
	int fileLength;

	// the reading range
	int beg, end;

	// log parsing keys
	const std::string error_key, warn_key, info_key;
	

	// data structure
	DataStructure data_struct;
};
