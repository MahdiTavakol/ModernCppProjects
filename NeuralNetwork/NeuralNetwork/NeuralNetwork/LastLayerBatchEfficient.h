#pragma once

#include "LayerBatchEfficient.h"
#include "Loss.h"

enum class LossType
{
	MSE,
	MAE,
	Huber
};


class LastLayerBatchEfficient : public LayerBatchEfficient
{
public:
	LastLayerBatchEfficient(const int& batchsize_,
		const int& inputDim_, const int& outputDim_,
		const ActivationType& activType_, const OptimizerType& optType_,
		const double& learningRate_, const LossType& lossType_);
	LastLayerBatchEfficient(const int& inputDim_, const int& outputDim_,
		const ActivationType& activType_, const OptimizerType& optType_,
		const double& learningRate_, const LossType& lossType_);
	virtual MatrixXd backward(MatrixXd& expectedValue_) override;
	double loss(const MatrixXd& output_, const MatrixXd& expectedValue_);

private:
	std::unique_ptr<Loss> lossFunction;
	LossType lossType;
};
