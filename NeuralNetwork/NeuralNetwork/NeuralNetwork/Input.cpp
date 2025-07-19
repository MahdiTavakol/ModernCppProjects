#include "Input.h"
#include <iostream>
#include <filesystem>

Input::Input(const string& dataFileName_):
	dataFile{dataFileName_}
{
	std::filesystem::path dataPath{ dataFileName_ };
	if (!std::filesystem::exists(dataPath))
		throw std::invalid_argument("Cannot open the file " + dataFileName_);
}

void Input::read()
{
	if (!dataFile.is_open())
		throw std::runtime_error("File is not open");
	dataFile.clear();         // clear any error flags
	dataFile.seekg(0);        // go back to the beginning
	fileDim = readCSVFileDim(dataFile);
	inputDim = fileDim;
	outputDim = inputDim;
	inputDim[1]--; // One col for the target
	outputDim[1] = 1; // One col for the target
	inputMatrix.resize(inputDim[0],inputDim[1]);
	outputMatrix.resize(outputDim[0], outputDim[1]);
	dataFile.clear();         // just to be extra safe
	dataFile.seekg(0);        // reset again before full read
	readCSVFile(dataFile, inputMatrix, outputMatrix, inputDim, outputDim);
}

void Input::return_data(array<int, 2> inputDim_, array<int, 2> outputDim_, MatrixXd& inputMatrix_, MatrixXd& outputMatrix_)
{
	if (inputMatrix.size() == 0 || outputMatrix.size() == 0)
	{
		std::cout << "Matrix has either not been read or has been retured as an rval" << std::endl;
		std::cout << " reading it again" << std::endl;
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
	if (!file_.is_open())
		throw std::runtime_error("The input file is closed!");

	string line,token;
	int numData = 0, dim = 0;
	array<int, 2> returnArray;
	VectorXd dataLineI;

	std::getline(file_, line);
	std::stringstream iss(line);
	
	while (std::getline(iss, token, ','))
		dim++;
	
	while (std::getline(file_, line))
		numData++;

	returnArray[0] = numData;
	returnArray[1] = dim;

	file_.clear();
	file_.seekg(0);

	return returnArray;
}

void Input::readCSVFile(ifstream& file_, MatrixXd& inputData_, MatrixXd& outputData_,
	array<int, 2>& inputDim_, array<int, 2> outputDim_, array<int, 2>& indxRange_)
{
	if (!file_.is_open())
		throw std::runtime_error("The input file is closed!");


	string line;
	VectorXd dataLineI;

	int numData = fileDim[0];
	int numCols = fileDim[1];

	if (indxRange_[0] >= fileDim[0] ||
		indxRange_[1] >= fileDim[0] ||
		indxRange_[0] < 0 ||
		indxRange_[1] < 0 )
		throw std::invalid_argument("The index is out of range");


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
		RowVectorXd inputRow{inputDim_[1]};
		RowVectorXd outputRow{outputDim_[1]};
		std::stringstream iss(line);
		while (std::getline(iss,readIntStr,','))
		{
			try
			{
				readInt = std::stoi(readIntStr);
			}
			catch (std::invalid_argument& e)
			{
				throw std::invalid_argument("Wrong input value in the csv file!");
			}
			countReadInt++;

			if (countReadInt <= inputDim_[1])
				inputRow[countReadInt - 1] = readInt;
			else if (countReadInt <= inputDim_[1] + outputDim_[1])
				outputRow[countReadInt - inputDim_[1] - 1] = readInt;
			else if (countReadInt > numCols)
			{
				std::cout << "Warning: The number of data in this line is larger than the dim" << std::endl;
				break;
			}
		}
		if (countReadInt < numCols)
			throw std::invalid_argument("Not enough data in the line");
		if (lineNumber >= numData) {
			std::cout << "Warning: The number of data in this file is larger " <<
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

