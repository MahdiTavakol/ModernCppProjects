#include "FinancialData.h"
#include <memory>
#include <map>

#define SORTDATA_LAMBDA_FUNC_ARGS [](const FinancialDataRecord& a, const FinancialDataRecord& b)

FinancialData::FinancialData(const std::string& filePath_):
	filePath(filePath_)
{
	std::unique_ptr<ReadData> readData = std::make_unique<ReadData>(filePath_);
	readData->read();
	readData->returnData(records);
	// At this stage the readData object contains null and it should not be called again otherwise the data will be read again.
}


void FinancialData::sortData(std::vector<FinancialDataRecord>& in_records, SortingKey key, SortingOrder order)
{
	static const std::map<SortingKey, std::function<bool(const FinancialDataRecord& a, const FinancialDataRecord& b)>> sortMapAscend =
	{
		{SortingKey::DATE,  SORTDATA_LAMBDA_FUNC_ARGS { return a.date   < b.date;  }},
		{SortingKey::TICKER,SORTDATA_LAMBDA_FUNC_ARGS { return a.ticker < b.ticker;}},
		{SortingKey::OPEN,  SORTDATA_LAMBDA_FUNC_ARGS { return a.open   < b.open;  }},
		{SortingKey::HIGH,  SORTDATA_LAMBDA_FUNC_ARGS { return a.high   < b.high;  }},
		{SortingKey::LOW,   SORTDATA_LAMBDA_FUNC_ARGS { return a.low    < b.low;  }},
		{SortingKey::CLOSE, SORTDATA_LAMBDA_FUNC_ARGS { return a.close  < b.close; }},
		{SortingKey::VOLUME,SORTDATA_LAMBDA_FUNC_ARGS { return a.volume < b.volume;}},
	};

	static const std::map<SortingKey, std::function<bool(const FinancialDataRecord& a, const FinancialDataRecord& b)>> sortMapDescend =
	{
		{SortingKey::DATE,  SORTDATA_LAMBDA_FUNC_ARGS { return a.date   > b.date;  }},
		{SortingKey::TICKER,SORTDATA_LAMBDA_FUNC_ARGS { return a.ticker > b.ticker; }},
		{SortingKey::OPEN,  SORTDATA_LAMBDA_FUNC_ARGS { return a.open   > b.open;  }},
		{SortingKey::HIGH,  SORTDATA_LAMBDA_FUNC_ARGS { return a.high   > b.high;  }},
		{SortingKey::LOW,   SORTDATA_LAMBDA_FUNC_ARGS { return a.low    > b.low;   }},
		{SortingKey::CLOSE, SORTDATA_LAMBDA_FUNC_ARGS { return a.close  > b.close; }},
		{SortingKey::VOLUME,SORTDATA_LAMBDA_FUNC_ARGS { return a.volume > b.volume; }},
	};

	
	static const std::map<SortingKey, std::function<bool(const FinancialDataRecord& a, const FinancialDataRecord& b)>>* sortMapPtr;
	switch (order)
	{
	case SortingOrder::ASCENDING:
		sortMapPtr = &sortMapAscend;
		break;
	case SortingOrder::DESCENDING:
		sortMapPtr = &sortMapDescend;
		break;
	default:
		throw std::runtime_error("Wrong order keyword");
	}

	auto it = sortMapPtr->find(key);
	if (it != sortMapPtr->end())
		std::ranges::sort(in_records, it->second);
	else
		throw std::invalid_argument("Unsupported sortingKey");
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

double FinancialData::maximum_drawdown(std::vector<FinancialDataRecord>& tickerRecords)
{
	std::string ticker_0 = tickerRecords[0].ticker;

	for (auto record : tickerRecords)
		if (record.ticker.compare(ticker_0))
		{
			/*
			    std::cout << "Warning: there are different tickers in the maximum_drawdowns" <<
				"filter each ticker separately by filterDataByTicker and for each " <<
				"ticker run maximum_drawdowns!" << std::endl;
			*/
			return -1.0;
		}

	sortData(tickerRecords, SortingKey::DATE);

	double maxDrawdown = 0.0;
	double currentDrawdown = 0.0;
	double peak = 0.0;



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
				currentDrawdown = (peak - record.close) / peak; // Calculate current drawdown
			}
		};

	std::ranges::for_each(tickerRecords, lambdaFunction);

	// Catch the last drawdown if it was the largest
	if (currentDrawdown > maxDrawdown) {
		maxDrawdown = currentDrawdown;
	}

	return maxDrawdown;
}

std::array<double, 2> FinancialData::calculate_average_stddev(const std::vector<FinancialDataRecord>& records_vector)
{
	std::array<double, 2> result = { 0.0,0.0 }; // {average, stddev}
	double& average = result[0];
	double& sttdev = result[1];
	average = std::accumulate(records_vector.begin(), records_vector.end(), 0.0,
		[](const double& acc, const FinancialDataRecord& record)
		{ return acc + record.close; }
	);
	average /= static_cast<double>(records_vector.size());
	sttdev = std::accumulate(records_vector.begin(), records_vector.end(), 0.0,
		[&average](const double& acc, const FinancialDataRecord& record) {
			return acc + (record.close - average) * (record.close - average);
		}) / static_cast<double>(records_vector.size() - 1);
	sttdev = std::sqrt(sttdev);
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

void FinancialData::writeDataToFile(const std::string& dataPath, const std::vector<FinancialDataRecord>& records_vector)
{
	std::ofstream outFile(dataPath);

	if (!outFile.is_open())
		throw std::runtime_error("Could not open file for writing: " + dataPath);

	std::string extension = dataPath.substr(dataPath.size() - 3);

	std::string header[] = {"Date","Ticker","Open","High","Low","Close","Volume"};
	char spacing;
	if (!extension.compare("txt")) spacing = ' ';
	else if (!extension.compare("csv")) spacing = ',';
	else throw std::invalid_argument("Unsupported file format " + extension);

	for (const auto& head : header)
		outFile << head << spacing;
	outFile << std::endl;

	for (const auto& record : records_vector) {
		outFile <<
			record.date << spacing <<
			record.ticker << spacing <<
			record.open << spacing <<
			record.high << spacing <<
			record.low << spacing <<
			record.close << spacing <<
			record.volume << std::endl;
	}
}