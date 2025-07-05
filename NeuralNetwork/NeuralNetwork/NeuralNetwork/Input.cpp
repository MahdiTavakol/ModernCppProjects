#include "Input.h"
#include <iostream>
#include <filesystem>

Input::Input(const string& inputFileName_, const string& outputFileName_):
	inputFile{inputFileName_}, outputFile{outputFileName_}
{
	std::filesystem::path inputPath{ inputFileName_ };
	std::filesystem::path outputPath{ outputFileName_ };
	if (!std::filesystem::exists(inputPath))
		throw std::invalid_argument("Cannot open the file " + inputFileName_);
	if (!std::filesystem::exists(outputPath))
		throw std::invalid_argument("Cannot open the file " + outputFileName_);
}

void Input::read()
{
	inputDim = readCSVFileDim(inputFile);
	outputDim = readCSVFileDim(outputFile);
	inputMatrix.resize(inputDim[0],inputDim[1]);
	outputMatrix.resize(outputDim[0], outputDim[1]);
	readCSVFile(inputFile, inputMatrix, inputDim);
	readCSVFile(outputFile, outputMatrix, outputDim);
}

void Input::return_data(array<int, 2> inputDim_, array<int, 2> outputDim_, MatrixXd& inputMatrix_, MatrixXd& outputMatrix_)
{
	if (!outputMatrix.rows() && !outputMatrix.cols())
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
	string line;
	int numData, dim;
	array<int, 2> returnArray;
	VectorXd dataLineI;

	std::getline(file_, line);
	std::stringstream iss(line);
	iss >> numData >> dim;

	returnArray[0] = dim;
	returnArray[1] = numData;

	return returnArray;
}

void Input::readCSVFile(ifstream& file_, MatrixXd& data_, array<int, 2>& dataDim_, array<int, 2>& indxRange_)
{
	string line;
	VectorXd dataLineI;

	int numData = dataDim_[0];
	int dim = dataDim_[1];

	if (indxRange_[0] >= dataDim_[0] || indxRange_[1] >= dataDim_[0])
		throw std::invalid_argument("The indx is out of range");


	int colNumber = -1;
	while (std::getline(file_, line))
	{
		colNumber++;
		if (colNumber < indxRange_[0] || colNumber > indxRange_[1])
			continue;
		int readInt;
		int countReadInt = 0;
		VectorXd readVector(dim);
		std::stringstream iss(line);

		while (iss >> readInt)
		{
			countReadInt++;
			if (countReadInt > dim)
				std::cout << "Warning: The number of data in this line is larger than the dim" << std::endl;
			readVector[countReadInt - 1] = readInt;
		}
		if (countReadInt < dim)
			throw std::invalid_argument("Not enough data in the line");

		data_.col(colNumber - indxRange_[0]) = readVector;

		if (colNumber >= numData) {
			std::cout << "Warning: The number of data in this file is larger " <<
				"than the number of data read from the first line: " <<
				"Ignoring the rest!" << std::endl;
			break;
		}
	}

	if (colNumber < numData)
		throw std::invalid_argument("Not enough data in the file");
}

