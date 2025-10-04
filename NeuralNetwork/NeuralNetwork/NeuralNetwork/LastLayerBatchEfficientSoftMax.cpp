#include "LastLayerBatchEfficientSoftMax.h"
#include <iostream>

LastLayerBatchEfficientSoftMax::LastLayerBatchEfficientSoftMax(Logger& logger_, const int& batchsize_,
	const int& InputFileDim_, const int& outputDim_,
	const ActivationType& activType_) :
	LayerBatchEfficient{logger_, batchsize_,InputFileDim_,outputDim_,activType_}
{}

LastLayerBatchEfficientSoftMax::LastLayerBatchEfficientSoftMax(Logger& logger_, const int& InputFileDim_, const int& outputDim_,
	const ActivationType& activType_) :
	LayerBatchEfficient{logger_,32,InputFileDim_, outputDim_, activType_}
{}

vector<MatrixXd>  LastLayerBatchEfficientSoftMax::active_diff(const MatrixXd& InputFile_)
{
	InputFile = InputFile_;
	//  (outputDimXInputFileDim)  X  (InputFileDimXBatchSize) -> outputDimXbatchsize
	MatrixXd linear = weights.block(0, 0, weights.rows(), weights.cols() - 1) * InputFile;
	//  (outputDimX1).replicate(1,batchsize) -> outputDimXbatchsize
	MatrixXd bias = weights.block(0, weights.cols() - 1, weights.rows(), 1).replicate(1, batchsize);
	// outputDimXbatchsize + outputDimXbatchsize -> outputDimXbatchsize
	MatrixXd z = linear + bias;
	SoftMaxLastLayer* softMaxActivPtr = dynamic_cast<SoftMaxLastLayer*>(activationFunction.get());
	if (softMaxActivPtr) throw std::runtime_error("The activation function is not of type SoftMaxLastLayer!");
	return softMaxActivPtr->diff_vec(z);
}

MatrixXd LastLayerBatchEfficientSoftMax::backward(MatrixXd& nextDiff_)
{
	if (activType != ActivationType::SOFTMAX)
		throw std::invalid_argument("Warning: This last layer is just for the softmax activation function!");
	// dActivation_doutput
	vector<MatrixXd> dActive_dz = active_diff(InputFile);

	// dLoss_dz
	MatrixXd dLoss_dz{outputDim,batchsize};
	
	for (int k = 0; k < batchsize; k++)
		dLoss_dz.col(k) = dActive_dz[k] * nextDiff_.col(k);


	dLoss_dweights.block(0, 0, weights.rows(), weights.cols() - 1) = dLoss_dz * InputFile.transpose();
	dLoss_dweights.block(0, weights.cols() - 1, weights.rows(), 1) = dLoss_dz.rowwise().sum();

	// previous_diff
	prev_diff = weights.block(0, 0, weights.rows(), weights.cols() - 1).transpose() * dLoss_dz;

	return prev_diff;
}
