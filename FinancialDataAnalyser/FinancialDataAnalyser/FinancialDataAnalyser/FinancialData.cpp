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

std::vector<FinancialDataRecord> FinancialData::filterDataByTicker(const std::string& ticker_)
{
	std::vector<FinancialDataRecord> filteredRecords;
	filteredRecords.reserve(records.size());
	std::ranges::copy_if(records, std::back_inserter(filteredRecords), [&ticker_](const FinancialDataRecord& rec) {
		return rec.ticker == ticker_; });
}

std::vector<FinancialDataRecord> FinancialData::filterDataByDate(const ChronoDate& date_)
{
	std::vector<FinancialDataRecord> filteredRecords;
	filteredRecords.reserve(records.size());
	std::ranges::copy_if(records, std::back_inserter(filteredRecords), [&date_](const FinancialDataRecord& rec) {
		return rec.date == date_; });
}