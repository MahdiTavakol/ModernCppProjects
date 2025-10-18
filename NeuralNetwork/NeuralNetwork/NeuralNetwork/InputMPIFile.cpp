#include "InputMPIFile.h"
#include <iostream>

InputMPIFile::InputMPIFile(Logger& logger_, const int& numTargetCols_, 
	                       const int& shuffleMode_, const int& shuffleSeed_) :
	InputFile{logger_, numTargetCols_, shuffleMode_, shuffleSeed_}
{}

InputMPIFile::InputMPIFile(Logger& logger_, const string& dataFileName_, const int& numTargetCols_,
	                       const int& shuffleMode_, const int& shuffleSeed_):
	InputFile{ logger_,dataFileName_,numTargetCols_,shuffleMode_,shuffleSeed_}
{
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
}

void InputMPIFile::read()
{
	if (rank == 0)
		if (logger.log_level >= LOG_LEVEL_INFO)
			logger << "Reading the file" << std::endl;
	localInputFileDim = InputFileDim;
	localOutputDim = outputDim;

	auto setRanges = [&](array<int, 2>& dim, array<int, 2>& range)
		{
			int perRank = (dim[0]+size-1) / size;
			range[0] = rank * perRank;
			range[1] = range[0] + perRank - 1 < dim[0] - 1 ? range[0] + perRank - 1 : dim[0] - 1;
			dim[0] = range[1] - range[0] + 1;
		};

	setRanges(localInputFileDim, InputFileRange);
	setRanges(localOutputDim, outputRange);
	InputFileMatrix.resize(localInputFileDim[0], localInputFileDim[1]);
	outputMatrix.resize(localOutputDim[0], localOutputDim[1]);


	dataFile.clear();         // clear any error flags
	dataFile.seekg(0);        // go back to the beginning
	readCSVFile(dataFile, InputFileMatrix, outputMatrix, localInputFileDim, localOutputDim, InputFileRange);

	if (rank == 0)
		if (logger.log_level >= LOG_LEVEL_INFO)
			logger << "Finished reading the file" << std::endl;
}