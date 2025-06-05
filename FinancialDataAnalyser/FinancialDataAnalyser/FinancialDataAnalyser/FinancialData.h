#pragma once

#include "ChronoDate.h"
#include "ReadData.h"
#include "FinancialDataRecord.h"
#include <string>
#include <memory>
#include <numeric>
#include <array>

class FinancialData {
public:
	FinancialData(const std::string& filePath_);
	void sortData(SortingKey key);
	std::vector<FinancialDataRecord> filterDataByTicker(const std::string& ticker) const;
	std::vector<FinancialDataRecord> filterDataByDate(const ChronoDate& date_) const;
	double maximum_drawdown(const std::string& ticker) const;
	std::array<double, 2> calculate_average_stddev(const std::string& ticker) const;
	double calculate_daily_return(const std::string& ticker) const;

protected:
	std::string filePath;
	std::vector<FinancialDataRecord> records;
};
