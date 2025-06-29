#pragma once
#include <Eigen/Dense>
#include "Activations.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;

enum class ActivationType
{
	RELU,
	SIGMOID,
	FASTSIGMOID,
	TANH
};

class Neuron
{
public:
	Neuron(const ActivationType& activType_);
	double forward(const VectorXd& input_);
	double backward(const VectorXd& input_);

private:
	VectorXd input;
	VectorXd weights;
	double bias;
	ActivationType activType;
	std::unique_ptr<Activation> activationFunction;
	double weightedSum, output, gradient;
};
