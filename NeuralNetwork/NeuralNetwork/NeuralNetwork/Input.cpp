#include "Input.h"
#include <iostream>

Input::Input(const string& inputFileName_, const string& outputFileName_):
	inputFile{inputFileName_}, outputFile{outputFileName_}
{
	if (!inputFile.is_open())
		throw std::invalid_argument("Cannot open the file " + inputFileName_);
	if (!outputFile.is_open())
		throw std::invalid_argument("Cannot open the file " + outputFileName_);
}

void Input::read()
{
	inputDim = readCSVFile(inputFile, inputMatrix);
	outputDim = readCSVFile(outputFile, outputMatrix);
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

array<int, 2> Input::readCSVFile(ifstream& file_, MatrixXd& output_)
{
	string line;
	int numData, dim;
	array<int, 2> returnArray;
	VectorXd dataLineI;

	std::getline(file_, line);
	std::stringstream iss(line);
	iss >> numData >> dim;

	output_.resize(dim, numData);


	int colNumber = -1;
	while (std::getline(file_, line))
	{
		colNumber++;
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

		output_.col(colNumber) = readVector;

		if (colNumber >= numData) {
			std::cout << "Warning: The number of data in this file is larger " <<
				"than the number of data read from the first line: " <<
				"Ignoring the rest!" << std::endl;
			break;
		}
	}

	if (colNumber < numData)
		throw std::invalid_argument("Not enough data in the file") << std::endl;

	returnArray[0] = dim;
	returnArray[1] = numData;

	return returnArray;
}

