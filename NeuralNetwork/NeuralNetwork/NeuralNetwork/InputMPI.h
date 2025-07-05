#pragma once
#include "Input.h"

#include <mpi.h>
#include <array>

using std::array;

class InputMPI : public Input
{
public:
	InputMPI(const string& inputFileName_, const string& outputFileName_);
	void read() override;

private:
	int size, rank;
	array<int, 2> inputRange, outputRange;
};