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

enum class SortingKey {
	DATE,
	TICKER,
	OPEN,
	HIGH,
	LOW,
	CLOSE,
	VOLUME
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
	void returnData(std::vector<FinancialDataRecord>& outRecords);

private:
	std::string filePath;
	std::ifstream inputStream;
	std::vector<FinancialDataRecord> records;
};