#include "InputFile.h"
#include <iostream>
#include <filesystem>
#include <numeric>

InputFile::InputFile(Logger& logger_, const int& numTargetCols_, const int& shuffleMode_, const int& shuffleSeed_) :
	logger{ logger_ }, shuffleMode{ shuffleMode_ }, shuffleSeed{ shuffleSeed_ }, numTargetCols{ numTargetCols_ }
{
}

InputFile::InputFile(Logger& logger_, const string& dataFileName_, const int& numTargetCols_,
	const int& shuffleMode_, const int& shuffleSeed_) :
	logger{ logger_ }, shuffleMode{ shuffleMode_ }, shuffleSeed{ shuffleSeed_ }, 
	dataFile{ dataFileName_ }, numTargetCols{ numTargetCols_ }
{
	std::filesystem::path dataPath{ dataFileName_ };
	if (!std::filesystem::exists(dataPath))
		throw std::invalid_argument("Cannot open the file " + dataFileName_);
	if (logger.log_level >= LOG_LEVEL_INFO)
		logger << "Opening the " << dataFileName_ << std::endl;
}

void InputFile::resetFileName(const string& dataFileName_)
{
	std::filesystem::path dataPath{ dataFileName_ };
	if (!std::filesystem::exists(dataPath))
		throw std::invalid_argument("Cannot open the file " + dataFileName_);
	dataFile.open(dataFileName_);
}

void InputFile::init()
{
	if (!dataFile.is_open())
		throw std::runtime_error("File is not open");
	dataFile.clear();         // clear any error flags
	dataFile.seekg(0);        // go back to the beginning
	fileDim = readCSVFileDim(dataFile);
	InputFileDim = fileDim;
	outputDim = InputFileDim;
	if (numTargetCols >= fileDim[1])
		throw std::invalid_argument("Number of Target columns is larger than the Data Columns!");
	InputFileDim[1] -= numTargetCols;
	outputDim[1] = numTargetCols;
	if (outputDim[1] > InputFileDim[1])
	{
		if (logger.log_level >= LOG_LEVEL_WARN)
			logger << "Warning: The number of target columns "
			<< outputDim[1]
			<< " is larger than the InputFile columns "
			<< InputFileDim[1]
			<< "!"
			<< std::endl;
	}

	// If no shuffle it just prints 0 to inputDim
	// ---> one to one mapping 
	shuffleIndex.resize(fileDim[0]);
	std::iota(shuffleIndex.begin(), shuffleIndex.end(), 0);

	if (shuffleMode & SHUFFLE) {
		std::shuffle(shuffleIndex.begin(), shuffleIndex.end(), shuffleSeed);
	}
}

void InputFile::read()
{
	InputFileMatrix.resize(InputFileDim[0], InputFileDim[1]);
	outputMatrix.resize(outputDim[0], outputDim[1]);
	dataFile.clear();         // just to be extra safe
	dataFile.seekg(0);        // reset again before full read
	readCSVFile(dataFile, InputFileMatrix, outputMatrix, InputFileDim, outputDim);
}

void InputFile::return_data(array<int, 2>& InputFileDim_, array<int, 2>& outputDim_, MatrixXd& InputFileMatrix_, MatrixXd& outputMatrix_)
{
	if (InputFileMatrix.size() == 0)
	{
		if (logger.log_level >= LOG_LEVEL_WARN)
		{
			logger << "Matrix has either not been read or has been retured as an rval" << std::endl;
			logger << " reading it again" << std::endl;
		}
		read();
	}

	returnDim(InputFileDim_, outputDim_);
	InputFileMatrix_ = std::move(InputFileMatrix);
	outputMatrix_ = std::move(outputMatrix);
	InputFileDim = array<int, 2>{ 0,0 };
	outputDim = array<int, 2>{ 0,0 };
}

array<int, 2> InputFile::readCSVFileDim(ifstream& file_)
{
	if (logger.log_level >= LOG_LEVEL_INFO)
		logger << "Getting the the file dimensions" << std::endl;
	if (!file_.is_open())
		throw std::runtime_error("The InputFile file is closed!");

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


	return returnArray;
}

void InputFile::readCSVFile(ifstream& file_, MatrixXd& InputFileData_, MatrixXd& outputData_,
	array<int, 2>& InputFileDim_, array<int, 2> outputDim_, array<int, 2>& indxRange_)
{
	if (logger.log_level >= LOG_LEVEL_INFO)
		logger << "Reading the file" << std::endl;
	if (!file_.is_open())
		throw std::runtime_error("The InputFile file is closed!");


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

		if (lineNumber - indxRange_[0] < 0 ||
			lineNumber - indxRange_[0] >= InputFileDim_[0] ||
			lineNumber - indxRange_[0] >= outputDim_[0])
			throw std::runtime_error("Out of range error: You should never have reached here!!!");

		const int indx = shuffleIndex[lineNumber] - indxRange_[0];
		if (indx > indxRange_[1] - indxRange_[0] && (shuffleMode & NO_SHUFFLE))
			break;
		else if ( (indx < 0 && (shuffleMode & NO_SHUFFLE)) ||
			      (indx < 0 && (shuffleMode & SHUFFLE)) ||
		          (indx > indxRange_[1] - indxRange_[0] && (shuffleMode & SHUFFLE)) )
			continue;
		else
			throw std::runtime_error("You never should have reached here!");

		int readInt;
		std::string readIntStr;
		int countReadInt = 0;
		RowVectorXd InputFileRow{ InputFileDim_[1] };
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
				oss << "Wrong InputFile value (" << readIntStr << ") in the csv file! " << e.what();
				throw std::invalid_argument(oss.str());
			}
			countReadInt++;

			if (countReadInt <= InputFileDim_[1])
				InputFileRow[countReadInt - 1] = readInt;
			else if (countReadInt <= InputFileDim_[1] + outputDim_[1])
				outputRow[countReadInt - InputFileDim_[1] - 1] = readInt;
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
		InputFileData_.row(indx) = InputFileRow;
		outputData_.row(indx) = outputRow;
	}
}
