#include "LayerBatchEfficient.h"

#include <random>


LayerBatchEfficient::LayerBatchEfficient(const int& batchsize_, const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_, const OptimizerType& optType_, const double& learningRate_) :
	batchsize{batchsize_},
	inputDim{ inputDim_ }, outputDim{ outputDim_ },
	input{inputDim_, batchsize_}, output{outputDim_,batchsize_},
	weights{outputDim_,inputDim_ + 1}, dLoss_dweights{outputDim_, inputDim_ + 1},
	prev_diff{inputDim_,batchsize_},
	activType{ activType_ }, 
	optType{ optType_ }, learningRate {learningRate_}
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
	MatrixXd output = weights.block(0, 0, weights.rows(), weights.cols() - 1) * input
		+ weights.block(0, weights.cols() - 1, weights.rows(), 1);
	output = output.unaryExpr([&](double v) {return (*activationFunction)(v); });
	return output;
}

MatrixXd LayerBatchEfficient::backward(MatrixXd& nextDiff_)
{
	// dActivation_doutput
	MatrixXd output = weights.block(0, 0, weights.rows(), weights.cols() - 1) * input 
		+ weights.block(0, weights.cols() - 1, weights.rows(), 1);

	MatrixXd dActive_doutput = output.unaryExpr([&](double v) {return activationFunction->diff(v); });

	// dLoss_dz
	MatrixXd dLoss_doutput = nextDiff_.cwiseProduct(dActive_doutput);

	dLoss_dweights.block(0,0, weights.rows(), weights.cols() - 1) = dLoss_doutput * input.transpose();
	dLoss_dweights.block(0, weights.cols() - 1, weights.rows(), 1) = dLoss_doutput.rowwise().sum();


	// previous_diff
	prev_diff = weights.block(0, 0, weights.rows(), weights.cols() - 1).transpose() * dLoss_doutput;

	return prev_diff;
}

void LayerBatchEfficient::update()
{
	optFunction->update(weights, dLoss_dweights);
}

void LayerBatchEfficient::weightInitialization(const double& mean_, const double& std_)
{
	std::random_device rd;
	std::mt19937_64 gen{ rd() };
	std::normal_distribution<double> dist{ mean_, std_ };

	weights.unaryExpr([&dist, &gen](const double v) {return dist(gen); });
}

MatrixXd&& LayerBatchEfficient::moveGradients()
{
	return std::move(dLoss_dweights);
}

MatrixXd LayerBatchEfficient::returnGradients()
{
	return dLoss_dweights;
}

array<int, 2> LayerBatchEfficient::returnInputOutputDims()
{
	array<int, 2> output;
	output[0] = inputDim;
	output[1] = outputDim;
	return output;
}

array<int, 2> LayerBatchEfficient::returnGradientSize()
{
	array<int, 2> dim;
	dim[0] = static_cast<int>(dLoss_dweights.rows());
	dim[1] = static_cast<int>(dLoss_dweights.cols());
	return dim;
}

void LayerBatchEfficient::updateGradients(MatrixXd&& gradients_)
{
	dLoss_dweights = std::move(gradients_);
}

