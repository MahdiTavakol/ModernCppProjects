#include "LayerBatchEfficient.h"

#include <random>
#include <iostream>


LayerBatchEfficient::LayerBatchEfficient(Logger& logger_, const int& batchsize_, const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_, const OptimizerType& optType_, const double& learningRate_) :
	logger{ logger_ },
	batchsize{ batchsize_ },
	inputDim{ inputDim_ }, outputDim{ outputDim_ },
	input{ inputDim_, batchsize_ }, output{ outputDim_,batchsize_ },
	dActive_dz{ outputDim_,batchsize_ },
	weights{ outputDim_,inputDim_ + 1 }, dLoss_dweights{ outputDim_, inputDim_ + 1 },
	prev_diff{ inputDim_,batchsize_ },
	activType{ activType_ },
	optType{ optType_ }, learningRate{ learningRate_ }
{
	switch (activType)
	{
	case ActivationType::NONE:
		activationFunction = std::make_unique<NoneAct>();
		break;
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
	case OptimizerType::NONE:
		optFunction = std::make_unique<NoneOpt>(learningRate);
		break;
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

LayerBatchEfficient::LayerBatchEfficient(Logger& logger_, const int& inputDim_, const int& outputDim_, const ActivationType& activType_,
	const OptimizerType& optType_, const double& learningRate_) :
	LayerBatchEfficient{ logger_,32,inputDim_,outputDim_,activType_,optType_,learningRate_ }
{}

void LayerBatchEfficient::initialize()
{
	double mean, std;
	int type = 0;
	switch (activType)
	{
	case ActivationType::TANH:
		type = 1;
		[[fallthrough]];
	case ActivationType::SIGMOID:
	{
		type = 2;
		mean = -std::sqrt(6.0 / static_cast<double>(inputDim + outputDim));
		std = std::sqrt(6.0 / static_cast<double>(inputDim + outputDim));
		break;
	}
	case ActivationType::NONE:
		[[fallthrough]];
	case ActivationType::RELU:
		type = 3;
		[[fallthrough]];
	case ActivationType::LEAKYRELU:
	{
		type = 4;
		mean = 0.0;
		std = std::sqrt(2.0 / inputDim);
		break;
	}
	case ActivationType::SELU:
	{
		type = 5;
		mean = 0.0;
		std = 1.0 / inputDim;
		break;
	}
	default:
		throw std::invalid_argument("Unknown activation funtion type!");
	}
	std::cout << "Activation function type= " << type << " and inptDim = " << inputDim << std::endl;
	
	gen.seed(rd());
	distPtr = std::make_unique<std::normal_distribution<double>>(mean,std);

	// Just weights has to be initialized not bias values
	weights.block(0,0,weights.rows(),weights.cols()-1) = weights.block(0,0,weights.rows(),weights.cols()-1).unaryExpr([&](const double /*v*/) {return (*distPtr)(gen); });
	weights.block(0,weights.cols()-1,weights.rows(),1) = MatrixXd::Constant(weights.rows(),1,0.02);
	
	

	//double weights_avg = weights.sum(); // /static_cast<double>(weights.cols()*weights.rows());
}

MatrixXd LayerBatchEfficient::forward(const MatrixXd& input_, const bool /*do not use*/)
{
	input = input_;
	//  (outputDimXinputDim)  X  (inputDimXBatchSize) -> outputDimXbatchsize
	MatrixXd linear = weights.block(0, 0, weights.rows(), weights.cols() - 1) * input;
	//  (outputDimX1).replicate(1,batchsize) -> outputDimXbatchsize
	// For the last batch the number of data (linear.cols()) is less than the batchsize 
	int batch_len = static_cast<int>(linear.cols());
	MatrixXd bias = weights.block(0, weights.cols() - 1, weights.rows(), 1).replicate(1, batch_len);
	// outputDimXbatchsize + outputDimXbatchsiz e -> outputDimXbatchsize
	MatrixXd z = linear + bias;

	// only resize if it necessary
	if (output.rows() != z.rows() ||
		output.cols() != z.cols())
		output.resize(z.rows(), z.cols());

	output = z.unaryExpr([&](double v) {return (*activationFunction)(v); });

	// needed for the backward step
	dActive_dz = z.unaryExpr([&](double v) {return activationFunction->diff(v); });


	/*
	std::cout << "Inside the forward for the layer" << std::endl;
	double linear_avg  = linear.sum()/static_cast<double>(linear.cols()*linear.rows());
	double bias_avg = bias.sum()/static_cast<double>(bias.cols()*bias.rows());
	double z_avg = z.sum()/static_cast<double>(z.cols()*z.rows());
	double input_avg = input.sum()/static_cast<double>(input.cols()*input.rows());
	double dActive_dz_avg = dActive_dz.sum()/static_cast<double>(dActive_dz.cols()*dActive_dz.rows());
	double weights_avg = weights.sum()/static_cast<double>(weights.cols()*weights.rows());
	std::cout << "linear_avg inside the layer= " << linear_avg << std::endl;
	std::cout << "bias_avg inside the layer= " << bias_avg << std::endl;
	std::cout << "z_avg inside the layer= " << z_avg << std::endl;
	std::cout << "input_avg inside the layer= " << input_avg << std::endl;
	std::cout << "dActive_dz_avg inside the layer= " << dActive_dz_avg << std::endl;
	std::cout << "weights_avg inside the layer= " << weights_avg << std::endl;
	*/

	return output;
}

// The nextDiff_ contains the diffLoss / diffOi of this layer for each data point 
// outputDim * batchsize
MatrixXd LayerBatchEfficient::backward(MatrixXd& nextDiff_)
{
	// output = Active(z) = Active(weights * input + bias)
	// here was are intested in three different diffs
	// 1 --> dLoss_dweigths (to optimize this layer weights)
	// 2 --> dLoss_dbiass (to optimize this layer bias)
	// 3 --> dLoss_dinput (prev_diff : to be passed as the nextDiff_ for the previous layer)
	// dActivation_dz has been calculated in the forward step

	// dLoss_dz (z is the output before the activation function is applied)
	MatrixXd dLoss_dz = nextDiff_.cwiseProduct(dActive_dz);

	dLoss_dweights.block(0, 0, weights.rows(), weights.cols() - 1) = dLoss_dz * input.transpose();
	dLoss_dweights.block(0, weights.cols() - 1, weights.rows(), 1) = dLoss_dz.rowwise().sum();
	
	// Add the L2 regularization
	dLoss_dweights += -L2regFactor*weights;

	// previous_diff
	prev_diff = weights.block(0, 0, weights.rows(), weights.cols() - 1).transpose() * dLoss_dz;


	/*
	std::cout << "Inside the backward for the layer" << std::endl;
	double nextdiff_avg  = nextDiff_.sum()/static_cast<double>(nextDiff_.cols()*nextDiff_.rows());
	double dactive_dz_avg = dActive_dz.sum()/static_cast<double>(dActive_dz.cols()*dActive_dz.rows());
	double dloss_dz_avg = dLoss_dz.sum()/static_cast<double>(dLoss_dz.cols()*dLoss_dz.rows());
	double input_avg = input.sum()/static_cast<double>(input.cols()*input.rows());
	*/
	// double avg = dLoss_dweights.sum()/(static_cast<double>(dLoss_dweights.cols())*static_cast<double>(dLoss_dweights.rows()));
	/*
	std::cout << "dLoss_dweights_avg inside the layer= " << avg << std::endl;
	std::cout << "Input_avg inside the layer= " << input_avg << std::endl;
	std::cout << "dloss_dz_avg inside the layer= " << dloss_dz_avg << std::endl;
	std::cout << "dactive_dz_avg inside the layer= " << dactive_dz_avg << std::endl;
	std::cout << "nextdiff_avg inside the layer= " << nextdiff_avg  << std::endl;
	*/
	/*if (std::abs(avg) > 0.01)
		std::cout << "dloss_dweights_avg inside the layer= " << avg << "The layerdims =" << inputDim << "," << outputDim << std::endl;
	*/

	return prev_diff;
}

void LayerBatchEfficient::update()
{
	optFunction->update(weights, dLoss_dweights);
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

