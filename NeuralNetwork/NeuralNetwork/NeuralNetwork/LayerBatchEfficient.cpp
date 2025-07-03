#include "LayerBatchEfficient.h"

#include <random>


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
	case ActivationType::LEAKYRELU:
		activationFunction = std::make_unique<LeakyRelu>();
		break;
	case ActivationType::SELU:
		activationFunction = std::make_unique<Selu>();
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

LayerBatchEfficient::LayerBatchEfficient(const int& inputDim_, const int& outputDim_, const ActivationType& activType_,
	const OptimizerType& optType_, const double& learningRate_):
	LayerBatchEfficient{32,inputDim_,outputDim_,activType_,optType_,learningRate_}
{}

void LayerBatchEfficient::initialize()
{
	double mean, std;
	switch (activType)
	{
	case ActivationType::TANH:
	case ActivationType::SIGMOID:
	{
		mean = -std::sqrt(6.0 / static_cast<double>(inputDim + outputDim));
		std = std::sqrt(6.0 / static_cast<double>(inputDim + outputDim));
		break;
	}
	case ActivationType::RELU:
	case ActivationType::LEAKYRELU:
	{
		mean = 0.0;
		std = 2.0 / inputDim;
		break;
	}
	case ActivationType::SELU:
	{
		mean = 0.0;
		std = 1.0 / inputDim;
		break;
	}
	default:
		throw std::invalid_argument("Unknown activation funtion type!");
	}
	weightInitialization(mean, std);
}

MatrixXd LayerBatchEfficient::forward(const MatrixXd& input_)
{
	MatrixXd biasMatrix = bias.replicate(1, batchsize);
	MatrixXd output = weights * input + biasMatrix;
	return output.unaryExpr((*activationFunction));
}

MatrixXd LayerBatchEfficient::backward(MatrixXd& nextDiff_)
{
	// dActivation_dz
	MatrixXd z = weights * input + bias;
	MatrixXd dActive_dz = z.unaryExpr([&](double v) {return activationFunction->diff(v); });

	// dLoss_dz
	MatrixXd dLoss_dz = nextDiff_.cwiseProduct(dActive_dz);

	dLoss_dweights = (dLoss_dz * input.transpose());

	// dzi_dbias
	dLoss_dbias = dLoss_dz.rowwise().sum();

	// previous_diff
	prev_diff = weights.transpose() * dLoss_dz;

	return prev_diff;
}

void LayerBatchEfficient::update()
{
	MatrixXd weightsExtended(weights.rows(), weights.cols() + 1);
	MatrixXd gradientExtended(weights.rows(), weights.cols() + 1);

	weightsExtended.col(0) = bias;
	weightsExtended.block(0, 1, weights.rows(), weights.cols());

	gradientExtended.col(0) = dLoss_dbias;
	gradientExtended.block(0, 1, dLoss_dweights.rows(), dLoss_dweights.cols());

	optFunction->update(weightsExtended, gradientExtended);

	bias = weightsExtended.col(0);
	weights = weightsExtended.block(0, 1, weightsExtended.rows(), weightsExtended.cols() - 1);
}

void LayerBatchEfficient::weightInitialization(const double& mean_, const double& std_)
{
	std::random_device rd;
	std::mt19937_64 gen{ rd() };
	std::normal_distribution<double> dist{ mean_, std_ };

	MatrixXd weightsExtended{ weights.rows(),weights.cols() + 1 };
	
	weightsExtended.unaryExpr([&dist, &gen](const double& v) {return dist(gen); });

	bias = weightsExtended.col(0);
	weights = weightsExtended.block(0, 1, weights.rows(), weights.cols());
}
