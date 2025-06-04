#include "ReadData.h"

ReadData::ReadData(const std::string& filePath_): 
	filePath{ filePath_ } {}

void ReadData::read()
{
	inputStream.open(filePath);
	// Check if the file is open
	if (!inputStream.is_open()) {
		throw std::runtime_error("Could not open file: " + filePath);
	}

	// Lets reserve space for 1000 records initially
	records.reserve(1000);

	std::string line;
	std::getline(inputStream, line);  // Skip the header line
	while (std::getline(inputStream, line))
	{
		std::stringstream ss(line);
		if (line.empty()) continue; // Skip empty lines
		FinancialDataRecord record;
		if (!(ss >> record.date >> record.ticker >> record.open >> record.high >> record.low >> record.close >> record.volume)) {
			throw std::runtime_error("Error reading line: " + line);
		}
		records.push_back(record);
	}
}

void ReadData::returnData(std::vector<FinancialDataRecord>& outRecords)
{
	if (records.empty())
	{
		std::cout << "Warning: Either no data has been read or it has previously been returned. Calling the read() first" << std::endl;
		this->read();
		if (records.empty()) {
			throw std::runtime_error("No data available after reading. Check the input file.");
	}
	outRecords = std::move(records);
}