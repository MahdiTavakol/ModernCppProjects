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
	double loss(const MatrixXd& output_,const MatrixXd& expected_);

protected:
	int batchsize;

	int inputDim, outputDim;
	MatrixXd input, output; // inputDim X batchsize , outputDim X batchsize

	// these are updated when each batch is fed into the layer!
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


	//weight initialization
	void weightInitialization(const double& mean_, const double& std_);

};