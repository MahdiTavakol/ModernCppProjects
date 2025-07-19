#pragma once
#include "Input.h"

#include <mpi.h>
#include <array>

using std::array;

class InputMPI : public Input
{
public:
	InputMPI(const string& dataFileName_);
	void read() override;

private:
	int size, rank;
	array<int, 2> inputRange, outputRange;
	array<int, 2> localInputDim;
	array<int, 2> localOutputDim;

};