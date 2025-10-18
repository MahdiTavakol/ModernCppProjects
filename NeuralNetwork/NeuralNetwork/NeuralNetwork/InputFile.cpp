#include "InputFile.h"
#include <iostream>
#include <filesystem>
#include <numeric>
#include <random>

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
		std::mt19937_64 gen(shuffleSeed);
		std::shuffle(shuffleIndex.begin(), shuffleIndex.end(), gen);
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
	while (std::getline(file_, line)) {
		++lineNumber;

		const int start = indxRange_[0];
		const int end = indxRange_[1];             // inclusive
		const int len = end - start + 1;           // number of rows in this range
		int idx;

		// Fast path for NO_SHUFFLE: skip before start, stop after end
		if (shuffleMode & NO_SHUFFLE) {
			if (lineNumber < start) continue;
			if (lineNumber > end) break;

			idx = lineNumber - start;      // 0..len-1
			// sanity guard against shape mistakes
			if (idx < 0 || idx >= InputFileData_.rows() || idx >= outputData_.rows())
				throw std::runtime_error("Index out of range (NO_SHUFFLE)");
			// ... parse tokens and write into row `idx` ...
			// (keep your parsing code below; use `idx` instead of `indx`)
			// ---------------- parsing block below will use `rowIndex = idx`
			// continue to token parsing...
			// (fall through after this block)
		} else { // SHUFFLE
			const int shuffled = shuffleIndex[lineNumber];

			// If this shuffled sample is not ours, skip
			if (shuffled < start || shuffled > end) {
				continue;
			}

			idx = shuffled - start;        // 0..len-1
			if (idx < 0 || idx >= len) {
				throw std::runtime_error("Index mapping bug (SHUFFLE)");
			}
			if (idx >= InputFileData_.rows() || idx >= outputData_.rows()) {
				throw std::runtime_error("Destination matrix row out of range (SHUFFLE)");
			}
			// ... parse tokens and write into row `idx` ...
			// (keep your parsing code below; use `idx` instead of `indx`)
		}

		// ↓↓↓ keep your existing token parsing, but write into `rowIndex = idx`
		// Example (unchanged except the target row variable):
		int numCols = fileDim[1];
		std::string tok;
		int count = 0;
		RowVectorXd inputRow(InputFileDim_[1]);
		RowVectorXd outputRow(outputDim_[1]);

		std::stringstream iss(line);
		std::getline(iss, tok, ','); // skip id

		while (std::getline(iss, tok, ',')) {
			// if your CSV can have floats, prefer std::stod
			int val = 0;
			try { val = std::stoi(tok); }
			catch (const std::invalid_argument& e) {
				std::ostringstream oss;
				oss << "Bad numeric value (" << tok << "): " << e.what();
				throw std::invalid_argument(oss.str());
			}
			++count;

			if (count <= InputFileDim_[1])           inputRow[count - 1] = val;
			else if (count <= InputFileDim_[1] + outputDim_[1])
				outputRow[count - InputFileDim_[1] - 1] = val;
			else if (count > numCols) {
				if (logger.log_level >= LOG_LEVEL_WARN)
					logger << "Warning: extra columns ignored\n";
				break;
			}
		}
		if (count < numCols) throw std::invalid_argument("Not enough data in the line");

		// finally write the rows
		InputFileData_.row(idx) = inputRow;
		outputData_.row(idx) = outputRow;
	}
}
