#include "LastLayerBatchEfficient.h"
#include <iostream>

LastLayerBatchEfficient::LastLayerBatchEfficient(const int& batchsize_, 
	const int& inputDim_, const int& outputDim_, 
	const ActivationType& activType_, const OptimizerType& optType_,
	const double& learningRate_, const LossType& lossType_):
	LayerBatchEfficient{batchsize_,inputDim_,outputDim_,activType_,optType_,learningRate_ }, lossType{lossType_}
{ 
	switch (lossType)
	{
		case LossType::MSE:
			lossFunction = std::make_unique<MSE>();
			break;
		case LossType::MAE:
			lossFunction = std::make_unique<MAE>();
			break;
		case LossType::Huber:
			lossFunction = std::make_unique<Huber>();
			break;
		default:
			throw std::invalid_argument("Wrong loss function type");
	}
}

LastLayerBatchEfficient::LastLayerBatchEfficient(const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_, const OptimizerType& optType_,
	const double& learningRate_, const LossType& lossType_):
	LastLayerBatchEfficient{32,inputDim_, outputDim_, activType_, optType_, learningRate_, lossType_ }
{}


MatrixXd LastLayerBatchEfficient::backward(MatrixXd& expectedValue_)
{
	// output = Active(z) = Active(weights * input + bias)
	// Error check
	if (activType == ActivationType::SOFTMAX)
		std::cout << "Warning: The use of softmax with the"
		<< " LastLayerBatchEfficient ignores the non-orthogonal "
		<< "components of differential of the activation function\n"
		<< "The special lastlayer designed for this purpose "
		<< "should be used!" << std::endl;
	// dActivation_doutput
	MatrixXd dActive_dz = forward(input, ActivMode::Diff);
	
	// dLoss_dz
	MatrixXd dLoss_dz = lossFunction->diff(expectedValue_, output).cwiseProduct(dActive_dz);

	dLoss_dweights.block(0, 0, weights.rows(), weights.cols() - 1) = dLoss_dz * input.transpose();
	dLoss_dweights.block(0, weights.cols() - 1, weights.rows(), 1) = dLoss_dz.rowwise().sum();

	// previous_diff
	prev_diff = weights.block(0, 0, weights.rows(), weights.cols() - 1).transpose() * dLoss_dz;


	return prev_diff;
}

double LastLayerBatchEfficient::loss(const MatrixXd& output_,const MatrixXd& expectedValue_)
{
	return (*lossFunction)(output_,expectedValue_);
}
