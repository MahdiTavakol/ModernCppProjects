#pragma once
#include <Eigen/Dense>
#include <array>

#include "Activations.h"
#include "Optimizers.h"
#include "Logger.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::RowVectorXd;
using std::array;

enum class ActivationType
{
	NONE,
	RELU,
	LEAKYRELU,
	SELU,
	SIGMOID,
	FASTSIGMOID,
	TANH,
	SOFTMAX
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
	LayerBatchEfficient(Logger& logger_, const int& batchsize_, const int& inputDim_, const int& outputDim_, const ActivationType& activType_,
		const OptimizerType& optType_, const double& learningRate_);
	LayerBatchEfficient(Logger& logger_, const int& inputDim_, const int& outputDim_, const ActivationType& activType,
		const OptimizerType& optType_, const double& learningRate_);
	virtual void initialize();
	virtual MatrixXd forward(const MatrixXd& input_); // outputDim X batchsize --> inputDim X batchsize
	virtual MatrixXd backward(MatrixXd& nextDiff_);
	void update();
	// To be used by MPI and possibly other parallelization approaches
	MatrixXd&& moveGradients();
	MatrixXd returnGradients();
	array<int, 2> returnInputOutputDims();
	array<int, 2> returnGradientSize();
	void updateGradients(MatrixXd&& gradients_);
	double returnDLoss_deweights(const int& i, const int& j) {
		return dLoss_dweights(i, j);
	}

protected:
	Logger& logger;
	int batchsize;

	int inputDim, outputDim;
	MatrixXd input, output; // inputDim X batchsize , outputDim X batchsize
	MatrixXd dActive_dz; // outputDim X batchsize

	// these are updated when each batch is fed into the layer!
	MatrixXd weights; // outputDim X (inputDim + 1)
	MatrixXd dLoss_dweights; // outputDim X (inputDim + 1)


	MatrixXd prev_diff; // inputDim X batchsize

	std::unique_ptr<Activation> activationFunction;
	ActivationType activType;

	std::unique_ptr<Optimizer> optFunction;
	OptimizerType  optType;

	double learningRate;
	double L2regFactor = 1e-5;

};
