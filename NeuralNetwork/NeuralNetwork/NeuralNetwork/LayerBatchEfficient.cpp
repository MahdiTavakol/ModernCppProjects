#include "LayerBatchEfficient.h"

#include <random>
#include <iostream>


LayerBatchEfficient::LayerBatchEfficient(Logger& logger_, const int& batchsize_, const int& InputFileDim_, const int& outputDim_,
	const ActivationType& activType_) :
	logger{ logger_ },
	batchsize{ batchsize_ },
	InputFileDim{ InputFileDim_ }, outputDim{ outputDim_ },
	InputFile{ InputFileDim_, batchsize_ }, output{ outputDim_,batchsize_ },
	dActive_dz{ outputDim_,batchsize_ },
	weights{ outputDim_,InputFileDim_ + 1 }, dLoss_dweights{ outputDim_, InputFileDim_ + 1 },
	prev_diff{ InputFileDim_,batchsize_ },
	activType{ activType_ }
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
}

LayerBatchEfficient::LayerBatchEfficient(Logger& logger_, const int& InputFileDim_, const int& outputDim_, const ActivationType& activType_) :
	LayerBatchEfficient{ logger_,32,InputFileDim_,outputDim_,activType_}
{}

void LayerBatchEfficient::initialize()
{
	int seed = rd();
	initialize(seed);
}

void LayerBatchEfficient::initialize(const int& seed_)
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
		mean = -std::sqrt(6.0 / static_cast<double>(InputFileDim + outputDim));
		std = std::sqrt(6.0 / static_cast<double>(InputFileDim + outputDim));
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
		std = std::sqrt(2.0 / InputFileDim);
		break;
	}
	case ActivationType::SELU:
	{
		type = 5;
		mean = 0.0;
		std = 1.0 / InputFileDim;
		break;
	}
	default:
		throw std::invalid_argument("Unknown activation funtion type!");
	}
	if (logger.log_level >= LOG_LEVEL_INFO)
		std::cout << "Activation function type= " << type << " and inptDim = " << InputFileDim << std::endl;
	
	gen.seed(seed_);
	distPtr = std::make_unique<std::normal_distribution<double>>(mean,std);

	// Just weights has to be initialized not bias values
	weights.block(0,0,weights.rows(),weights.cols()-1) = weights.block(0,0,weights.rows(),weights.cols()-1).unaryExpr([&](const double /*v*/) {return (*distPtr)(gen); });
	weights.block(0,weights.cols()-1,weights.rows(),1) = MatrixXd::Constant(weights.rows(),1,0.02);
	
	

	//double weights_avg = weights.sum(); // /static_cast<double>(weights.cols()*weights.rows());
}

MatrixXd LayerBatchEfficient::forward(const MatrixXd& InputFile_, const bool /*do not use*/)
{
	InputFile = InputFile_;
	//  (outputDimXInputFileDim)  X  (InputFileDimXBatchSize) -> outputDimXbatchsize
	MatrixXd linear = weights.block(0, 0, weights.rows(), weights.cols() - 1) * InputFile;
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
	double InputFile_avg = InputFile.sum()/static_cast<double>(InputFile.cols()*InputFile.rows());
	double dActive_dz_avg = dActive_dz.sum()/static_cast<double>(dActive_dz.cols()*dActive_dz.rows());
	double weights_avg = weights.sum()/static_cast<double>(weights.cols()*weights.rows());
	std::cout << "linear_avg inside the layer= " << linear_avg << std::endl;
	std::cout << "bias_avg inside the layer= " << bias_avg << std::endl;
	std::cout << "z_avg inside the layer= " << z_avg << std::endl;
	std::cout << "InputFile_avg inside the layer= " << InputFile_avg << std::endl;
	std::cout << "dActive_dz_avg inside the layer= " << dActive_dz_avg << std::endl;
	std::cout << "weights_avg inside the layer= " << weights_avg << std::endl;
	*/

	return output;
}

// The nextDiff_ contains the diffLoss / diffOi of this layer for each data point 
// outputDim * batchsize
MatrixXd LayerBatchEfficient::backward(MatrixXd& nextDiff_)
{
	// output = Active(z) = Active(weights * InputFile + bias)
	// here was are intested in three different diffs
	// 1 --> dLoss_dweigths (to optimize this layer weights)
	// 2 --> dLoss_dbiass (to optimize this layer bias)
	// 3 --> dLoss_dInputFile (prev_diff : to be passed as the nextDiff_ for the previous layer)
	// dActivation_dz has been calculated in the forward step

	// dLoss_dz (z is the output before the activation function is applied)
	MatrixXd dLoss_dz = nextDiff_.cwiseProduct(dActive_dz);

	dLoss_dweights.block(0, 0, weights.rows(), weights.cols() - 1) = (dLoss_dz * InputFile.transpose())/static_cast<double>(InputFile.cols());
	dLoss_dweights.block(0, weights.cols() - 1, weights.rows(), 1) = dLoss_dz.rowwise().mean();
	
	// Add the L2 regularization
	dLoss_dweights += L2regFactor*weights;

	// previous_diff
	prev_diff = weights.block(0, 0, weights.rows(), weights.cols() - 1).transpose() * dLoss_dz;


	/*
	std::cout << "Inside the backward for the layer" << std::endl;
	double nextdiff_avg  = nextDiff_.sum()/static_cast<double>(nextDiff_.cols()*nextDiff_.rows());
	double dactive_dz_avg = dActive_dz.sum()/static_cast<double>(dActive_dz.cols()*dActive_dz.rows());
	double dloss_dz_avg = dLoss_dz.sum()/static_cast<double>(dLoss_dz.cols()*dLoss_dz.rows());
	double InputFile_avg = InputFile.sum()/static_cast<double>(InputFile.cols()*InputFile.rows());
	*/
	// double avg = dLoss_dweights.sum()/(static_cast<double>(dLoss_dweights.cols())*static_cast<double>(dLoss_dweights.rows()));
	/*
	std::cout << "dLoss_dweights_avg inside the layer= " << avg << std::endl;
	std::cout << "InputFile_avg inside the layer= " << InputFile_avg << std::endl;
	std::cout << "dloss_dz_avg inside the layer= " << dloss_dz_avg << std::endl;
	std::cout << "dactive_dz_avg inside the layer= " << dactive_dz_avg << std::endl;
	std::cout << "nextdiff_avg inside the layer= " << nextdiff_avg  << std::endl;
	*/
	/*if (std::abs(avg) > 0.01)
		std::cout << "dloss_dweights_avg inside the layer= " << avg << "The layerdims =" << InputFileDim << "," << outputDim << std::endl;
	*/

	return prev_diff;
}

void LayerBatchEfficient::update(const std::unique_ptr<Optimizer>& OptFuncPtr_)
{
	OptFuncPtr_->update(weights, dLoss_dweights);
}

MatrixXd&& LayerBatchEfficient::moveGradients()
{
	return std::move(dLoss_dweights);
}

MatrixXd LayerBatchEfficient::returnGradients()
{
	return dLoss_dweights;
}

array<int, 2> LayerBatchEfficient::returnInputFileOutputDims()
{
	array<int, 2> output;
	output[0] = InputFileDim;
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

