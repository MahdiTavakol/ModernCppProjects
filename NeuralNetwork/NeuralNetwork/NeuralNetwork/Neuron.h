#pragma once
#include <Eigen/Dense>
#include "Activations.h"
#include "Loss.h"

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
	Neuron(const ActivationType& activType_, const double& learningRate_);
	void reset(const VectorXd& weights_, const VectorXd& input_);
	double forward();
	void backward(const double& diffPrevious);
	void update();

private:
	double learningRate;
	VectorXd input, dLoss_dweights;
	VectorXd weights;
	double bias, dLoss_dbias;
	ActivationType activType;
	std::unique_ptr<Activation> activationFunction;
	std::unique_ptr<Loss> LossFunction;
	double weightedSum, output, gradient;
};
