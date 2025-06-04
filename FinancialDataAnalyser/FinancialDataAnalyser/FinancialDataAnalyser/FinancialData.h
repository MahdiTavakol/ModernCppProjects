#pragma once

#include "ChronoDate.h"
#include "ReadData.h"
#include "FinancialDataRecord.h"
#include <string>
#include <memory>

class FinancialData {
public:
	FinancialData(const std::string& filePath_);
	void sortData(SortingKey key);
	std::vector<FinancialDataRecord> filterDataByTicker(const std::string& ticker);
	std::vector<FinancialDataRecord> filterDataByDate(const ChronoDate& date_);

protected:
	std::string filePath;
	std::vector<FinancialDataRecord> records;
};
