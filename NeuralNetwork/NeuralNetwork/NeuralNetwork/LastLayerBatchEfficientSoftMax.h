#pragma once

#include "LayerBatchEfficient.h"


class LastLayerBatchEfficientSoftMax final : public LayerBatchEfficient
{
public:
	LastLayerBatchEfficientSoftMax(Logger& logger_, const int& batchsize_,
		const int& InputFileDim_, const int& outputDim_,
		const ActivationType& activType_);
	LastLayerBatchEfficientSoftMax(Logger& logger_, const int& InputFileDim_, const int& outputDim_,
		const ActivationType& activType_);
	vector<MatrixXd> active_diff(const MatrixXd& InputFile_); // outputDim X batchsize --> (InputFileDimXInputFileDim) X batchsize 
	MatrixXd backward(MatrixXd& expectedValue_) override;

};