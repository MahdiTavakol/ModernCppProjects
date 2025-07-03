#pragma once

#include "LayerBatchEfficient.h"
#include "Loss.h"

enum class LossType
{
	MSE,
	MAE,
	Huber
};


class LastLayerBatchEfficient final : public LayerBatchEfficient
{
public:
	LastLayerBatchEfficient(const int& inputDim_, const int& outputDim_, const ActivationType& activType_,
		const double& learningRate_, const LossType& lossType_);
	MatrixXd backward(MatrixXd& expectedValue_) override;
	double loss(MatrixXd& output_, MatrixXd& expectedValue_);

private:

	std::unique_ptr<Loss> lossFunction;
	LossType lossType;
};
