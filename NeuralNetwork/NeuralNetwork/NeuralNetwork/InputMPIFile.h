#pragma once
#include "InputFile.h"

#include <mpi.h>
#include <array>

using std::array;

class InputMPIFile : public InputFile
{
public:
	InputMPIFile(Logger& logger, const int& numTargetCols_);
	InputMPIFile(Logger& logger, const string& dataFileName_, const int& numTargetCols_);
	void read() override;
private:
	int size, rank;
	array<int, 2> InputFileRange, outputRange;
	array<int, 2> localInputFileDim;
	array<int, 2> localOutputDim;
};