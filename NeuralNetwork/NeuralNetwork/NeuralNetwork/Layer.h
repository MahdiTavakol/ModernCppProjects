#pragma once
#include <memory>
#include <Eigen/Dense>
#include <vector>
#include "Neuron.h"

using Eigen::VectorXd;
using std::vector;

class Layer
{
public:
	Layer(const int& inputDim_, const int& outputDim_);


private:
	int inputDim, outputDim;
	VectorXd input, output;

	vector<Neuron> neurons;
};