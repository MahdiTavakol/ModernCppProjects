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
	void initialize(const VectorXd& weights_, const VectorXd& input_);
	double forward();
	double backward();

private:
	VectorXd input;
	VectorXd weights;
	double bias;
	ActivationType activType;
	std::unique_ptr<Activation> activationFunction;
	double weightedSum, output, gradient;
};
