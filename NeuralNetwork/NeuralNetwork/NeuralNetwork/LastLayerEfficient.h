#pragma once

#include "LayerEfficient.h"
#include "Loss.h"

enum class LossType
{
	MSE,
	MAE,
	Huber
};


class LastLayerEfficient final : public LayerEfficient
{
public:
	LastLayerEfficient(const int& inputDim_, const int& outputDim_, const ActivationType& activType_,
		const double& learningRate_, const LossType& lossType_);
	void backward(VectorXd& expectedValue_) override;

private:

	std::unique_ptr<Loss> lossFunction;
	LossType lossType;
};
