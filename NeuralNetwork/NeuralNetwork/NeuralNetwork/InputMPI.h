#pragma once
#include "Input.h"

#include <mpi.h>
#include <array>

using std::array;

class InputMPI : public Input
{
public:
	InputMPI(Logger& logger, const int& numTargetCols_);
	InputMPI(Logger& logger, const string& dataFileName_, const int& numTargetCols_);
	void read() override;
private:
	int size, rank;
	array<int, 2> inputRange, outputRange;
	array<int, 2> localInputDim;
	array<int, 2> localOutputDim;
};