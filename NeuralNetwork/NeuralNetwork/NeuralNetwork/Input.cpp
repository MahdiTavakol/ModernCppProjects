#include "Input.h"
#include <iostream>
#include <filesystem>

Input::Input(Logger& logger_, const int& numTargetCols_) :
	logger{ logger_ }, numTargetCols{ numTargetCols_ }
{
}

Input::Input(Logger& logger_, const string& dataFileName_, const int& numTargetCols_) :
	logger{ logger_ }, dataFile{ dataFileName_ }, numTargetCols{ numTargetCols_ }
{
	std::filesystem::path dataPath{ dataFileName_ };
	if (!std::filesystem::exists(dataPath))
		throw std::invalid_argument("Cannot open the file " + dataFileName_);
	logger << "Opening the " << dataFileName_ << std::endl;
}

void Input::resetFileName(const string& dataFileName_)
{
	std::filesystem::path dataPath{ dataFileName_ };
	if (!std::filesystem::exists(dataPath))
		throw std::invalid_argument("Cannot open the file " + dataFileName_);
	dataFile.open(dataFileName_);
}

void Input::init()
{
	if (!dataFile.is_open())
		throw std::runtime_error("File is not open");
	dataFile.clear();         // clear any error flags
	dataFile.seekg(0);        // go back to the beginning
	fileDim = readCSVFileDim(dataFile);
	inputDim = fileDim;
	outputDim = inputDim;
	if (numTargetCols >= fileDim[1])
		throw std::invalid_argument("Number of Target columns is larger than the Data Columns!");
	inputDim[1] -= numTargetCols;
	outputDim[1] = numTargetCols;
	if (outputDim[1] > inputDim[1])
	{
		if (logger.log_level >= LOG_LEVEL_WARN)
			logger << "Warning: The number of target columns "
			<< outputDim[1]
			<< " is larger than the input columns "
			<< inputDim[1]
			<< "!"
			<< std::endl;
	}
}

void Input::read()
{
	inputMatrix.resize(inputDim[0], inputDim[1]);
	outputMatrix.resize(outputDim[0], outputDim[1]);
	dataFile.clear();         // just to be extra safe
	dataFile.seekg(0);        // reset again before full read
	readCSVFile(dataFile, inputMatrix, outputMatrix, inputDim, outputDim);
}

void Input::return_data(array<int, 2>& inputDim_, array<int, 2>& outputDim_, MatrixXd& inputMatrix_, MatrixXd& outputMatrix_)
{
	if (inputMatrix.size() == 0)
	{
		if (logger.log_level >= LOG_LEVEL_WARN)
		{
			logger << "Matrix has either not been read or has been retured as an rval" << std::endl;
			logger << " reading it again" << std::endl;
		}
		read();
	}

	returnDim(inputDim_, outputDim_);
	inputMatrix_ = std::move(inputMatrix);
	outputMatrix_ = std::move(outputMatrix);
	inputDim = array<int, 2>{ 0,0 };
	outputDim = array<int, 2>{ 0,0 };
}

array<int, 2> Input::readCSVFileDim(ifstream& file_)
{
	if (logger.log_level >= LOG_LEVEL_INFO)
		logger << "Getting the the file dimensions" << std::endl;
	if (!file_.is_open())
		throw std::runtime_error("The input file is closed!");

	string line, token;
	int numData = 0, dim = 0;
	array<int, 2> returnArray;
	VectorXd dataLineI;

	std::getline(file_, line);
	std::stringstream iss(line);

	while (std::getline(iss, token, ',')) {
		dim++;
	}

	dim--; // The first col is the id

	while (std::getline(file_, line))
		numData++;

	returnArray[0] = numData;
	returnArray[1] = dim;

	if (logger.log_level >= LOG_LEVEL_INFO)
		logger << "There are " << numData << " data points each with " << dim << " features" << std::endl;

	file_.clear();
	file_.seekg(0);

	returnArray[0] = 320;

	return returnArray;
}

void Input::readCSVFile(ifstream& file_, MatrixXd& inputData_, MatrixXd& outputData_,
	array<int, 2>& inputDim_, array<int, 2> outputDim_, array<int, 2>& indxRange_)
{
	if (logger.log_level >= LOG_LEVEL_INFO)
		logger << "Reading the file" << std::endl;
	if (!file_.is_open())
		throw std::runtime_error("The input file is closed!");


	string line;
	VectorXd dataLineI;

	int numData = fileDim[0];
	int numCols = fileDim[1];

	if (indxRange_[0] >= fileDim[0] ||
		indxRange_[1] >= fileDim[0] ||
		indxRange_[0] < 0 ||
		indxRange_[1] < 0)
		throw std::invalid_argument("The index is out of range");

	// Skipping the header line.
	std::getline(file_, line);


	int lineNumber = -1;
	while (std::getline(file_, line))
	{
		lineNumber++;
		if (lineNumber < indxRange_[0])
			continue;
		if (lineNumber > indxRange_[1])
			break;
		int readInt;
		std::string readIntStr;
		int countReadInt = 0;
		RowVectorXd inputRow{ inputDim_[1] };
		RowVectorXd outputRow{ outputDim_[1] };
		std::stringstream iss(line);
		std::ostringstream oss;
		std::getline(iss, readIntStr, ',');
		while (std::getline(iss, readIntStr, ','))
		{
			try
			{
				readInt = std::stoi(readIntStr);
			}
			catch (std::invalid_argument& e)
			{
				oss << "Wrong input value (" << readIntStr << ") in the csv file! " << e.what();
				throw std::invalid_argument(oss.str());
			}
			countReadInt++;

			if (countReadInt <= inputDim_[1])
				inputRow[countReadInt - 1] = readInt;
			else if (countReadInt <= inputDim_[1] + outputDim_[1])
				outputRow[countReadInt - inputDim_[1] - 1] = readInt;
			else if (countReadInt > numCols)
			{
				if (logger.log_level >= LOG_LEVEL_WARN)
					logger << "Warning: The number of data in this line is larger than the dim" << std::endl;
				break;
			}
		}
		if (countReadInt < numCols)
			throw std::invalid_argument("Not enough data in the line");
		if (lineNumber >= numData) {
			if (logger.log_level >= LOG_LEVEL_WARN)
				logger << "Warning: The number of data in this file is larger " <<
				"than the number of data read from the first line: " <<
				"Ignoring the rest!" << std::endl;
			break;
		}
		if (lineNumber - indxRange_[0] < 0 ||
			lineNumber - indxRange_[0] >= inputDim_[0] ||
			lineNumber - indxRange_[0] >= outputDim_[0])
			throw std::runtime_error("Out of range error: You should never have reached here!!!");
		inputData_.row(lineNumber - indxRange_[0]) = inputRow;
		outputData_.row(lineNumber - indxRange_[0]) = outputRow;
	}
}
