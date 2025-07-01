#pragma once
#include <Eigen/Dense>

#include "Activations.h"
#include "Optimizers.h"

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

enum class OptimizerType
{
	SGD,
	SGDMOMENUM,
	RMSPROP,
	ADAM,
	ADAGRAD
};


class LayerEfficient
{
public:
	LayerEfficient(const int& inputDim_, const int& outputDim_, const ActivationType& activType_, const OptimizerType& optType_, const double& learningRate_);
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

	std::unique_ptr<Optimizer> optFunction;
	OptimizerType  optType;

	double learningRate;
};