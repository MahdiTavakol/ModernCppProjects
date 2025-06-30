#include "LayerEfficient.h"

LayerEfficient::LayerEfficient(const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_, const double& learningRate_) :
	inputDim{ inputDim_ }, outputDim{ outputDim_ },
	activType{ activType_ }, learningRate{ learningRate_ }
{
	switch (activType)
	{
	case ActivationType::RELU:
		activationFunction = std::make_unique<ReLu>();
		break;
	case ActivationType::SIGMOID:
		activationFunction = std::make_unique<Sigmoid>();
		break;
	case ActivationType::FASTSIGMOID:
		activationFunction = std::make_unique<FastSigmoid>();
		break;
	case ActivationType::TANH:
		activationFunction = std::make_unique<TanH>();
		break;
	default:
		throw std::invalid_argument("Unknown loss type!");
	}
}

VectorXd LayerEfficient::forward(VectorXd& input_)
{
	VectorXd output = weights * input + bias;
	return output.unaryExpr((*activationFunction));
}

void LayerEfficient::backward(VectorXd& nextDiff_)
{
	// dActivation_dz
	VectorXd z = weights * input + bias;
	VectorXd dActive_dz = z.unaryExpr([&](double v) {return activationFunction->diff(v); });
	// dz_dweights
	RowVectorXd dz_dweights = input.transpose();
	// dLoss_dz
	VectorXd dLoss_dz = nextDiff_.cwiseProduct(dActive_dz);

	dLoss_dweights = dLoss_dz * dz_dweights;

	// dzi_dbias
	VectorXd dz_dbias = Eigen::VectorXd::Ones(inputDim);
	dLoss_dbias = dLoss_dz * dz_dbias;

	// previous_diff
	prev_diff = weights.transpose() * dLoss_dz;
}

void LayerEfficient::update()
{
	weights -= learningRate * dLoss_dweights;
	bias -= learningRate * dLoss_dbias;
}