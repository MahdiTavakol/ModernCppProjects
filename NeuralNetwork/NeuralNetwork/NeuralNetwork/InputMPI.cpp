#include "InputMPI.h"

InputMPI::InputMPI(const string& inputFileName_, const string& outputFileName_):
	Input{inputFileName_,outputFileName_}
{
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
}

void InputMPI::read()
{
	inputDim = readCSVFileDim(inputFile);
	outputDim = readCSVFileDim(outputFile);

	auto setRanges = [&](array<int, 2>& dim, array<int, 2>& range)
		{
			int perRank = dim[0] / size;
			range[0] = rank * perRank;
			range[1] = range[0] + perRank < dim[0] ? range[0] + perRank : dim[0];
			dim[0] = range[1] - range[0] + 1;
		};

	setRanges(inputDim, inputRange);
	setRanges(outputDim, outputRange);

	inputMatrix.resize(inputDim[0], inputDim[1]);
	outputMatrix.resize(outputDim[0], outputDim[1]);
	readCSVFile(inputFile, inputMatrix, inputDim, inputRange);
	readCSVFile(outputFile, outputMatrix, outputDim, outputRange);
}