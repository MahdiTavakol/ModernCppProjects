#include "ReadData.h"

ReadData::ReadData(const std::string& filePath_): 
	filePath{ filePath_ }
{
	std::string extension = filePath.substr(filePath.size() - 3);

	if (!extension.compare("txt")) fileMode = FileMode::TXT;
	else if (!extension.compare("csv")) fileMode = FileMode::CSV;
	else throw std::invalid_argument("Unsupported file format " + extension);
}

void ReadData::read()
{
	inputStream.open(filePath);
	// Check if the file is open
	if (!inputStream.is_open()) {
		throw std::runtime_error("Could not open file: " + filePath);
	}
	// Lets reserve space for 1000 records initially
	records.reserve(1000);

	switch (fileMode)
	{
	case FileMode::TXT:
		read_txt(inputStream);
		break;
	case FileMode::CSV:
		read_csv(inputStream);
		break;
	default:
		throw std::runtime_error("error");
	}
}

void ReadData::read_txt(std::istream& inputStream)
{
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

void ReadData::read_csv(std::istream& inputStream)
{
	
	std::string line;
	std::getline(inputStream, line);  // Skip the header line
	while (std::getline(inputStream, line))
	{
		std::stringstream ss(line);
		std::string word;
		if (line.empty()) continue; // Skip empty lines
		FinancialDataRecord record;


		auto std_getline_double = [](std::stringstream& sss, double& out)
			{
				std::string word;
				if (!std::getline(sss, word, ','))
					throw std::runtime_error("Error reading");
				else
				{
					std::stringstream sss2(word);
					sss2 >> out;
				}
			};

		auto std_getline_int = [](std::stringstream& sss, int& out)
			{
				std::string word;
				if (!std::getline(sss, word, ','))
					throw std::runtime_error("Error reading");
				else
				{
					std::stringstream sss2(word);
					sss2 >> out;
				}
			};

		if (!std::getline(ss, word, ','))
			throw std::runtime_error("Error reading line: " + line);
		else
		{
			std::stringstream ss2(word);
			ss2 >> record.date;
		}
		if (!std::getline(ss, word, ','))
			throw std::runtime_error("Error reading line: " + line);
		else
		{
			std::stringstream ss2(word);
			ss2 >> record.ticker;
		}

		std_getline_double(ss, record.open);
		std_getline_double(ss, record.high);
		std_getline_double(ss, record.low);
		std_getline_double(ss, record.close);
		std_getline_int(ss, record.volume);

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
}