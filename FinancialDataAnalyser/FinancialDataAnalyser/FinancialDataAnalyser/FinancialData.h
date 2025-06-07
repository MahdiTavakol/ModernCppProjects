#pragma once

#include "ChronoDate.h"
#include "ReadData.h"
#include "FinancialDataRecord.h"
#include <string>
#include <memory>
#include <numeric>
#include <array>

#define TICKER_RECORD std::vector<FinancialDataRecord> tickerRecords = filterDataByTicker(ticker_)

enum class SortingOrder
{
	ASCENDING = 0,
	DESCENDING = 1
};

class FinancialData {
public:
	FinancialData(const std::string& filePath_);

	FinancialData(const FinancialData&) = default;
	FinancialData& operator=(const FinancialData&) = default;
	FinancialData(FinancialData&&) = default;
	FinancialData& operator=(const FinancialData&&) = default;
	~FinancialData() = default;

	std::vector<FinancialDataRecord> filterDataByTicker(const std::string& ticker) const;
	std::vector<FinancialDataRecord> filterDataByDate(const ChronoDate& date_) const;

	double calculate_daily_return(const std::string& ticker) const;


	void sortData(SortingKey key) {
		sortData(records, key);
	}
	double maximum_drawdown(const std::string& ticker_) const
	{
		TICKER_RECORD;
		return maximum_drawdown(tickerRecords);
	}
	void writeDataToFile(const std::string& dataPath) const {
		writeDataToFile(dataPath, records);
	}
	std::array<double, 2> calculate_average_stddev(const std::string& ticker_) const
	{
		TICKER_RECORD;
		return calculate_average_stddev(tickerRecords);
	}

	static void sortData(std::vector<FinancialDataRecord>& in_records, SortingKey key, SortingOrder order = SortingOrder::ASCENDING);
	static double maximum_drawdown(std::vector<FinancialDataRecord>& tickerRecords);
	static void writeDataToFile(const std::string& dataPath, const std::vector<FinancialDataRecord>& records_vector);
	static std::array<double, 2> calculate_average_stddev(const std::vector<FinancialDataRecord>& records_vector);

protected:
	std::string filePath;
	std::vector<FinancialDataRecord> records;
};
