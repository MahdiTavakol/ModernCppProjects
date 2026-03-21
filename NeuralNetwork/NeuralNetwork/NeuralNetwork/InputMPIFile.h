#pragma once
#include "InputFile.h"

#include <mpi.h>
#include <array>

using std::array;

class InputMPIFile : public InputFile
{
public:
	InputMPIFile(Logger& logger, const int& numTargetCols_, const int& shuffleMode_ = NO_SHUFFLE, const int& shuffleSeed_ = 123);
	InputMPIFile(Logger& logger, const string& dataFileName_, const int& numTargetCols_, const int& shuffleMode_ = NO_SHUFFLE, const int& shuffleSeed_ = 123);
	void read() override;
private:
	int size, rank;
	array<int, 2> InputFileRange, outputRange;
	array<int, 2> localInputFileDim;
	array<int, 2> localOutputDim;
};