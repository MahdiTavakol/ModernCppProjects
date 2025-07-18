#include "InputMPI.h"

InputMPI::InputMPI(const int& numTargetCols_) :
	Input{ numTargetCols_ }
{}

InputMPI::InputMPI(const string& dataFileName_, const int& numTargetCols_):
	Input{dataFileName_,numTargetCols_}
{
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
}

void InputMPI::read()
{
	localInputDim = inputDim;
	localOutputDim = outputDim;

	auto setRanges = [&](array<int, 2>& dim, array<int, 2>& range)
		{
			int perRank = (dim[0]+size-1) / size;
			range[0] = rank * perRank;
			range[1] = range[0] + perRank - 1 < dim[0] - 1 ? range[0] + perRank - 1 : dim[0] - 1;
			dim[0] = range[1] - range[0] + 1;
		};

	setRanges(localInputDim, inputRange);
	setRanges(localOutputDim, outputRange);
	inputMatrix.resize(localInputDim[0], localInputDim[1]);
	outputMatrix.resize(localOutputDim[0], localOutputDim[1]);


	dataFile.clear();         // clear any error flags
	dataFile.seekg(0);        // go back to the beginning
	readCSVFile(dataFile, inputMatrix, outputMatrix, localInputDim, localOutputDim, inputRange);
}