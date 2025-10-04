#pragma once

#include "LastLayerBatchEfficient.h"


class LastLayerBatchEfficientSoftMax final : public LastLayerBatchEfficient
{
public:
	LastLayerBatchEfficientSoftMax(Logger& logger_, const int& batchsize_,
		const int& InputFileDim_, const int& outputDim_,
		const ActivationType& activType_, const OptimizerType& optType_,
		const double& learningRate_, const LossType& lossType_);
	LastLayerBatchEfficientSoftMax(Logger& logger_, const int& InputFileDim_, const int& outputDim_,
		const ActivationType& activType_, const OptimizerType& optType_,
		const double& learningRate_, const LossType& lossType_);
	vector<MatrixXd> active_diff(const MatrixXd& InputFile_); // outputDim X batchsize --> (InputFileDimXInputFileDim) X batchsize 
	MatrixXd backward(MatrixXd& expectedValue_) override;

private:
	std::unique_ptr<Loss> lossFunction;
	LossType lossType;
};