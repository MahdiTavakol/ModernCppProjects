#include "LastLayerBatchEfficient.h"
#include <iostream>

LastLayerBatchEfficient::LastLayerBatchEfficient(Logger& logger_, const int& batchsize_,
	const int& InputFileDim_, const int& outputDim_,
	const ActivationType& activType_, const OptimizerType& optType_,
	const double& learningRate_, const LossType& lossType_) :
	LayerBatchEfficient{ logger_,batchsize_,InputFileDim_,outputDim_,activType_,optType_,learningRate_ }, lossType{ lossType_ }
{
	switch (lossType)
	{
	case LossType::MSE:
		lossFunction = std::make_unique<MSE>();
		break;
	case LossType::MAE:
		lossFunction = std::make_unique<MAE>();
		break;
	case LossType::Huber:
		lossFunction = std::make_unique<Huber>();
		break;
	default:
		throw std::invalid_argument("Wrong loss function type");
	}
}

LastLayerBatchEfficient::LastLayerBatchEfficient(Logger& logger_, const int& InputFileDim_, const int& outputDim_,
	const ActivationType& activType_, const OptimizerType& optType_,
	const double& learningRate_, const LossType& lossType_) :
	LastLayerBatchEfficient{ logger_,32,InputFileDim_, outputDim_, activType_, optType_, learningRate_, lossType_ }
{
}


MatrixXd LastLayerBatchEfficient::backward(MatrixXd& expectedValue_)
{
	// output = Active(z) = Active(weights * InputFile + bias)
	// Error check
	if (activType == ActivationType::SOFTMAX)
		logger << "Warning: The use of softmax with the"
		<< " LastLayerBatchEfficient ignores the non-orthogonal "
		<< "components of differential of the activation function\n"
		<< "The special lastlayer designed for this purpose "
		<< "should be used!" << std::endl;
	// dActivation_dz calculated during the forward step
	// dLoss_dz
	MatrixXd dLoss_dz = lossFunction->diff(output,expectedValue_).cwiseProduct(dActive_dz);
	dLoss_dweights.block(0, 0, weights.rows(), weights.cols() - 1) = dLoss_dz * InputFile.transpose();
	dLoss_dweights.block(0, weights.cols() - 1, weights.rows(), 1) = dLoss_dz.rowwise().sum();
	// previous_diff
	prev_diff = weights.block(0, 0, weights.rows(), weights.cols() - 1).transpose() * dLoss_dz;

	
	/*
	MatrixXd test = lossFunction->diff(expectedValue_, output);
	double loss_diff_avg = test.sum()/static_cast<double>(test.cols()*test.rows());
	double expected_avg  = expectedValue_.sum()/static_cast<double>(expectedValue_.cols()*expectedValue_.rows());
	double dactive_dz_avg = dActive_dz.sum()/static_cast<double>(dActive_dz.cols()*dActive_dz.rows());
	double dloss_dz_avg = dLoss_dz.sum()/static_cast<double>(dLoss_dz.cols()*dLoss_dz.rows());
	double InputFile_avg = InputFile.sum()/static_cast<double>(InputFile.cols()*InputFile.rows());
	*/
	//double avg = dLoss_dweights.sum()/static_cast<double>(dLoss_dweights.cols()*dLoss_dweights.rows());
	/*
	std::cout << "loss_diff_avg = " << loss_diff_avg << std::endl;
	std::cout << "expected_avg inside the layer= " << expected_avg << std::endl;
	std::cout << "InputFile_avg inside the layer= " << InputFile_avg << std::endl;
	std::cout << "dloss_dz_avg inside the layer= " << dloss_dz_avg << std::endl;
	std::cout << "dactive_dz_avg inside the layer= " << dactive_dz_avg << std::endl;
	*/
	/*if (std::abs(avg) > 0.01)
		std::cout << "dloss_dweights_avg inside the layer= " << avg << "The layerdims =" << InputFileDim << "," << outputDim << std::endl;
		*/

	return prev_diff;
}

double LastLayerBatchEfficient::loss(const MatrixXd& output_, const MatrixXd& expectedValue_)
{
	return (*lossFunction)(output_, expectedValue_);
}
