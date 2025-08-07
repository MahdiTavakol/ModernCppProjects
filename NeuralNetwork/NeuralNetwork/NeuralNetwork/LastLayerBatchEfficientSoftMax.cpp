#include "LastLayerBatchEfficientSoftMax.h"
#include <iostream>

LastLayerBatchEfficientSoftMax::LastLayerBatchEfficientSoftMax(Logger& logger_, const int& batchsize_,
	const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_, const OptimizerType& optType_,
	const double& learningRate_, const LossType& lossType_) :
	LastLayerBatchEfficient{logger_, batchsize_,inputDim_,outputDim_,activType_,optType_,learningRate_,lossType_ }
{}

LastLayerBatchEfficientSoftMax::LastLayerBatchEfficientSoftMax(Logger& logger_, const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_, const OptimizerType& optType_,
	const double& learningRate_, const LossType& lossType_) :
	LastLayerBatchEfficient{logger_,32,inputDim_, outputDim_, activType_, optType_, learningRate_, lossType_ }
{}

vector<MatrixXd>  LastLayerBatchEfficientSoftMax::active_diff(const MatrixXd& input_)
{
	input = input_;
	//  (outputDimXinputDim)  X  (inputDimXBatchSize) -> outputDimXbatchsize
	MatrixXd linear = weights.block(0, 0, weights.rows(), weights.cols() - 1) * input;
	//  (outputDimX1).replicate(1,batchsize) -> outputDimXbatchsize
	MatrixXd bias = weights.block(0, weights.cols() - 1, weights.rows(), 1).replicate(1, batchsize);
	// outputDimXbatchsize + outputDimXbatchsize -> outputDimXbatchsize
	MatrixXd z = linear + bias;
	SoftMaxLastLayer* softMaxActivPtr = dynamic_cast<SoftMaxLastLayer*>(activationFunction.get());
	if (softMaxActivPtr) throw std::runtime_error("The activation function is not of type SoftMaxLastLayer!");
	return softMaxActivPtr->diff_vec(z);
}

MatrixXd LastLayerBatchEfficientSoftMax::backward(MatrixXd& expectedValue_)
{
	if (activType != ActivationType::SOFTMAX)
		throw std::invalid_argument("Warning: This last layer is just for the softmax activation function!");
	// dActivation_doutput
	vector<MatrixXd> dActive_dz = active_diff(input);

	// dLoss_dz
	MatrixXd dLoss_dz{outputDim,batchsize};
	MatrixXd dLoss_doutput = lossFunction->diff(expectedValue_, output);
	
	for (int k = 0; k < batchsize; k++)
		dLoss_dz.col(k) = dActive_dz[k] * dLoss_doutput.col(k);


	dLoss_dweights.block(0, 0, weights.rows(), weights.cols() - 1) = dLoss_dz * input.transpose();
	dLoss_dweights.block(0, weights.cols() - 1, weights.rows(), 1) = dLoss_dz.rowwise().sum();

	// previous_diff
	prev_diff = weights.block(0, 0, weights.rows(), weights.cols() - 1).transpose() * dLoss_dz;

	return prev_diff;
}
