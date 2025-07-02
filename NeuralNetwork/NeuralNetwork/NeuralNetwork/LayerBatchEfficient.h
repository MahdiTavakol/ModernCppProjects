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


class LayerBatchEfficient
{
public:
	LayerBatchEfficient(const int& batchsize_, const int& inputDim_, const int& outputDim_, const ActivationType& activType_, const OptimizerType& optType_, const double& learningRate_);
	MatrixXd forward(MatrixXd& input_); // outputDim X batchsize --> inputDim X batchsize
	virtual void backward(MatrixXd& nextDiff_);
	void update();

protected:
	int batchsize;

	int inputDim, outputDim;
	MatrixXd input, output; // inputDim X batchsize , outputDim X batchsize
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