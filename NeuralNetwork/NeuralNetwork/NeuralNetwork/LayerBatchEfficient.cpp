#include "LayerBatchEfficient.h"

LayerBatchEfficient::LayerBatchEfficient(const int& batchsize_, const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_, const OptimizerType& optType_, const double& learningRate_) :
	batchsize{batchsize_},
	inputDim{ inputDim_ }, outputDim{ outputDim_ },
	activType{ activType_ }, 
	optType{ optType_ }, learningRate {
	learningRate_
}
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

	switch (optType)
	{
	case OptimizerType::SGD:
		optFunction = std::make_unique<SGD>(learningRate);
		break;
	case OptimizerType::SGDMOMENUM:
		optFunction = std::make_unique<SGDMomentum>(learningRate);
		break;
	case OptimizerType::RMSPROP:
		optFunction = std::make_unique<RMSProp>(learningRate);
		break;
	case OptimizerType::ADAM:
		optFunction = std::make_unique<Adam>(learningRate);
		break;
	case OptimizerType::ADAGRAD:
		optFunction = std::make_unique<AdaGrad>(learningRate);
		break;
	default:
		throw std::invalid_argument("Unknown optimizer type!");
	}

}

MatrixXd LayerBatchEfficient::forward(MatrixXd& input_)
{
	MatrixXd biasMatrix = bias.replicate(1, batchsize);
	MatrixXd output = weights * input + biasMatrix;
	return output.unaryExpr((*activationFunction));
}

void LayerBatchEfficient::backward(MatrixXd& nextDiff_)
{
	// dActivation_dz
	MatrixXd z = weights * input + bias;
	MatrixXd dActive_dz = z.unaryExpr([&](double v) {return activationFunction->diff(v); });

	// dLoss_dz
	MatrixXd dLoss_dz = nextDiff_.cwiseProduct(dActive_dz);

	dLoss_dweights = (dLoss_dz * input.transpose()) / batchsize;

	// dzi_dbias
	dLoss_dbias = dLoss_dz.rowwise().mean();

	// previous_diff
	prev_diff = weights.transpose() * dLoss_dz;
}

void LayerEfficient::update()
{
	MatrixXd newWeights(weights.rows(), weights.cols() + 1);
	newWeights.col(0) = dLoss_dbias;
	newWeights.block(0, 1, dLoss_dweights.rows(), dLoss_dweights.cols());
	
	optFunction->update(newWeights);

	bias = newWeights.col(0);
	weights = newWeights.block(0, 1, newWeights.rows(), newWeights.cols() - 1);
}