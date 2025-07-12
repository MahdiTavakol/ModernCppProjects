#pragma once
#include <Eigen/Dense>
#include <array>

#include "Activations.h"
#include "Optimizers.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::RowVectorXd;
using std::array;

enum class ActivationType
{
	RELU,
	LEAKYRELU,
	SELU,
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
	LayerBatchEfficient(const int& batchsize_, const int& inputDim_, const int& outputDim_, const ActivationType& activType_,
		const OptimizerType& optType_, const double& learningRate_);
	LayerBatchEfficient(const int& inputDim_, const int& outputDim_, const ActivationType& activType,
		const OptimizerType& optType_, const double& learningRate_);
	void initialize();
	MatrixXd forward(const MatrixXd& input_); // outputDim X batchsize --> inputDim X batchsize
	virtual MatrixXd backward(MatrixXd& nextDiff_);
	void update();
	// To be used by MPI and possibly other parallelization approaches
	MatrixXd&& moveGradients();
	MatrixXd returnGradients();
	array<int, 2> returnInputOutputDims();
	array<int, 2> returnGradientSize();
	void updateGradients(MatrixXd&& gradients_);

protected:
	int batchsize;

	int inputDim, outputDim;
	MatrixXd input, output; // inputDim X batchsize , outputDim X batchsize

	// these are updated when each batch is fed into the layer!
	MatrixXd weights; // outputDim X (inputDim + 1)
	MatrixXd dLoss_dweights; // outputDim X (inputDim + 1)


	VectorXd prev_diff; // inputDim X batchsize

	std::unique_ptr<Activation> activationFunction;
	ActivationType activType;

	std::unique_ptr<Optimizer> optFunction;
	OptimizerType  optType;

	double learningRate;


	//weight initialization
	void weightInitialization(const double& mean_, const double& std_);

};