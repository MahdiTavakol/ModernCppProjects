#include "ChronoDate.h"
#include "FinancialDataRecord.h"
#include "FinancialData.h"
#include "ReadData.h"

#include <iostream>
#include <memory>

void sorting_data(std::vector<FinancialDataRecord>& data_vector, SortingKey key, const std::string& title);
void checking_data(std::vector<FinancialDataRecord>& data_vector, const std::string& title);

void main()
{
	std::cout << "Setting up the financialDataCSV object" << std::endl;
	std::unique_ptr<FinancialData> financialDataCSV = std::make_unique<FinancialData>("sample_data.csv");
	std::cout << "Sorting the financialDataCSV object by date" << std::endl;
	financialDataCSV->sortData(SortingKey::DATE);

	// Checking ticker data
	struct
	{
		std::string ticker;
		SortingKey key;
	} inputs_ticker [] = {
		{"AMZN",SortingKey::CLOSE},
		{"GOOG",SortingKey::OPEN},
		{"AAPL",SortingKey::VOLUME},
		{"MSFT",SortingKey::HIGH},
		{"TSLA",SortingKey::LOW},
        {"AMZN",SortingKey::DATE}
	};

	// Checking date data
	struct
	{
		std::string date_string;
		SortingKey key;
	} inputs_date[] = {
		{"2023-04-03",SortingKey::CLOSE},
		{"2023-04-07",SortingKey::OPEN},
		{"2023-04-17",SortingKey::VOLUME},
		{"2023-04-29",SortingKey::HIGH},
		{"2023-05-13",SortingKey::LOW},
		{"2023-06-02",SortingKey::DATE}
	};

	std::cout << "Checking data by tickers" << std::endl;
	for (auto& input : inputs_ticker)
	{
		std::vector<FinancialDataRecord> data_vector = financialDataCSV->filterDataByTicker(input.ticker);
		sorting_data(data_vector, input.key, input.ticker);
		checking_data(data_vector, input.ticker);
	}

	std::cout << "Checking data by dates" << std::endl;
	for (auto& input : inputs_date)
	{
		std::stringstream ss(input.date_string);
		ChronoDate date;
		ss >> date;
		std::vector<FinancialDataRecord> data_vector = financialDataCSV->filterDataByDate(date);
		sorting_data(data_vector, input.key, input.date_string);
		checking_data(data_vector, input.date_string);
	}
}

void sorting_data(std::vector<FinancialDataRecord>& data_vector, SortingKey key, const std::string& title)
{
	FinancialData::sortData(data_vector, key);
	std::string sortKey_string = std::to_string(static_cast<int>(key));
	FinancialData::writeDataToFile("data/" + title + "_data_sorted_" + sortKey_string + ".csv", data_vector);
}

void checking_data(std::vector<FinancialDataRecord>& data_vector, const std::string& title)
{
	double data_max_drawdown = FinancialData::maximum_drawdown(data_vector);
	std::array<double, 2> data_avg_stddev = FinancialData::calculate_average_stddev(data_vector);
	if (std::abs(data_max_drawdown + 1.0) < 1e-5)
		std::cout << std::format("{} Average: {:.2f}, Standard Deviation: {:.2f}",
			title, data_avg_stddev[0], data_avg_stddev[1]) << std::endl;
	else
		std::cout << std::format("{} Average: {:.2f}, Standard Deviation: {:.2f}, Maximum Drawdown: {:.2f}",
			title, data_avg_stddev[0], data_avg_stddev[1], data_max_drawdown) << std::endl;
}