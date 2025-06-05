#include "FinancialData.h"
#include <memory>

FinancialData::FinancialData(const std::string& filePath_):
	filePath(filePath_)
{
	std::unique_ptr<ReadData> readData = std::make_unique<ReadData>(filePath_);
	readData->read();
	readData->returnData(records);
	// At this stage the readData object contains null and it should not be called again otherwise the data will be read again.
}


void FinancialData::sortData(SortingKey key)
{
	std::function<bool(const FinancialDataRecord& a, const FinancialDataRecord& b)> sortFunction;
	switch (key)
	{
	case SortingKey::DATE:
		sortFunction = [](const FinancialDataRecord& a, const FinancialDataRecord& b) {
			return a.date < b.date;
			};
		break;
	case SortingKey::TICKER:
		sortFunction = [](const FinancialDataRecord& a, const FinancialDataRecord& b) {
			return a.ticker < b.ticker;
			};
		break;
	case SortingKey::OPEN:
		sortFunction = [](const FinancialDataRecord& a, const FinancialDataRecord& b) {
			return a.open < b.open;
			};
		break;
	case SortingKey::HIGH:
		sortFunction = [](const FinancialDataRecord& a, const FinancialDataRecord& b) {
			return a.high < b.high;
			};
		break;
	case SortingKey::LOW:
		sortFunction = [](const FinancialDataRecord& a, const FinancialDataRecord& b) {
			return a.low < b.low;
			};
		break;
	case SortingKey::CLOSE:
		sortFunction = [](const FinancialDataRecord& a, const FinancialDataRecord& b) {
			return a.close < b.close;
			};
		break;
	case SortingKey::VOLUME:
		sortFunction = [](const FinancialDataRecord& a, const FinancialDataRecord& b) {
			return a.volume < b.volume;
			};
		break;
	default:
		throw std::invalid_argument("Invalid sorting key provided.");
	}
	std::ranges::sort(records, sortFunction);
}

std::vector<FinancialDataRecord> FinancialData::filterDataByTicker(const std::string& ticker_) const
{
	std::vector<FinancialDataRecord> filteredRecords;
	filteredRecords.reserve(records.size());
	std::ranges::copy_if(records, std::back_inserter(filteredRecords), [&ticker_](const FinancialDataRecord& rec) {
		return rec.ticker == ticker_; });
	return filteredRecords;
}

std::vector<FinancialDataRecord> FinancialData::filterDataByDate(const ChronoDate& date_) const
{
	std::vector<FinancialDataRecord> filteredRecords;
	filteredRecords.reserve(records.size());
	std::ranges::copy_if(records, std::back_inserter(filteredRecords), [&date_](const FinancialDataRecord& rec) {
		return rec.date == date_; });
	return filteredRecords;
}


double FinancialData::maximum_drawdown(const std::string& ticker_) const
{
	double maxDrawdown = 0.0;
	double currentDrawdown = 0.0;
	double peak = 0.0;

	std::vector<FinancialDataRecord> tickerRecords = filterDataByTicker(ticker_);

	std::function<void(const FinancialDataRecord& record)> lambdaFunction =
		[&maxDrawdown, &currentDrawdown, &peak](const FinancialDataRecord& record)
		{
			if (record.close > peak) {
				if (currentDrawdown > maxDrawdown) {
				maxDrawdown = currentDrawdown; // Update max drawdown if current is greater
			}
			currentDrawdown = 0.0; // Reset current drawdown if a new peak is found
			peak = record.close; // Update the peak if the current close is higher
			}
				else if (record.close < peak)
			{
				currentDrawdown = (peak - record.close)/peak; // Calculate current drawdown
			}
		};

	std::ranges::for_each(tickerRecords, lambdaFunction);

	// Catch the last drawdown if it was the largest
	if (currentDrawdown > maxDrawdown) {
		maxDrawdown = currentDrawdown;
	}

	return maxDrawdown;
}

std::array<double, 2> FinancialData::calculate_average_stddev(const std::string& ticker) const
{
	std::array<double, 2> result = { 0.0,0.0 }; // {average, stddev}
	double& average = result[0];
	double& sttdev = result[1];
	double average = std::accumulate(records.begin(), records.end(), 0.0,
		[](const double& acc, const FinancialDataRecord& record) 
		{ return acc + record.close; }
	);
	average /= static_cast<double>(records.size());
	double sttdev = std::accumulate(records.begin(), records.end(), 0.0,
		[&average](const double& acc, const FinancialDataRecord& record) {
			return acc + (record.close - average) * (record.close - average);
		}) / static_cast<double>(records.size() - 1);
	return result;
}

double FinancialData::calculate_daily_return(const std::string& ticker) const
{
	double totalReturn = 0.0;
	int count = 0;

	std::vector<FinancialDataRecord> tickerRecords = filterDataByTicker(ticker);

	if (tickerRecords.size() < 2) {
		throw std::runtime_error("Not enough data to calculate daily return for ticker: " + ticker);
	}

	auto total_return_func = [&totalReturn,&count](const FinancialDataRecord& current, const FinancialDataRecord& previous) {
		totalReturn += (current.close - previous.close) / previous.close;
		count++;
		};

	for (size_t i = 1; i < tickerRecords.size(); ++i) {
		const FinancialDataRecord& current = tickerRecords[i];
		const FinancialDataRecord& previous = tickerRecords[i - 1];

		if (current.date == previous.date) {
			continue; // Skip if the dates are the same
		}

		total_return_func(current, previous);
	}

	return totalReturn / count; // Return the average daily return
}