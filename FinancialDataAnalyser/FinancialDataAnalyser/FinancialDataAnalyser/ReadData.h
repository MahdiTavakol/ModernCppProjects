#pragma once

#include "ChronoDate.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "FinancialDataRecord.h"

enum class FileMode {
	TXT,
	CSV
};


class ReadData {
public:
	ReadData(const std::string& filePath_);
	ReadData(const ReadData&) = delete; // Disable copy constructor
	ReadData& operator=(const ReadData&) = delete; // Disable copy assignment operator
	ReadData(ReadData&&) = delete; // Disable move constructor
	ReadData& operator=(ReadData&&) = delete; // Disable move assignment operator
	~ReadData() = default; // Default destructor

	void read();
	void read_txt(std::istream& inputStream);
	void read_csv(std::istream& inputStream);
	void returnData(std::vector<FinancialDataRecord>& outRecords);

private:
	std::string filePath;
	FileMode fileMode;
	std::ifstream inputStream;
	std::vector<FinancialDataRecord> records;
};