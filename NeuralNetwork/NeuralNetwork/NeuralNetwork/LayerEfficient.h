#pragma once
#include <Eigen/Dense>

#include "Activations.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::RowVectorXd;

enum class ActivationType
{
	RELU,
	SIGMOID,
	FASTSIGMOID,
	TANH
};


class LayerEfficient
{
public:
	LayerEfficient(const int& inputDim_, const int& outputDim_, const ActivationType& activType_, const double& learningRate_);
	VectorXd forward(VectorXd& input_);
	virtual void backward(VectorXd& nextDiff_);
	void update();

protected:
	int inputDim, outputDim;
	VectorXd input, output;
	MatrixXd weights;
	VectorXd bias;
	MatrixXd dLoss_dweights;
	VectorXd dLoss_dbias;
	VectorXd prev_diff;

	std::unique_ptr<Activation> activationFunction;
	ActivationType activType;


	double learningRate;
};