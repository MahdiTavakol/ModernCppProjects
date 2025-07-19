#include "LastLayerBatchEfficient.h"

LastLayerBatchEfficient::LastLayerBatchEfficient(const int& batchsize_, 
	const int& inputDim_, const int& outputDim_, 
	const ActivationType& activType_, const OptimizerType& optType_,
	const double& learningRate_, const LossType& lossType_):
	LayerBatchEfficient{batchsize_,inputDim_,outputDim_,activType_,optType_,learningRate_ }, lossType{lossType_}
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

LastLayerBatchEfficient::LastLayerBatchEfficient(const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_, const OptimizerType& optType_,
	const double& learningRate_, const LossType& lossType_):
	LastLayerBatchEfficient{32,inputDim_, outputDim_, activType_, optType_, learningRate_, lossType_ }
{}


MatrixXd LastLayerBatchEfficient::backward(MatrixXd& expectedValue_)
{
	// dActivation_doutput
	MatrixXd output = weights.block(0, 0, weights.rows(), weights.cols() - 1) * input
		+ weights.block(0, weights.cols() - 1, weights.rows(), 1);

	MatrixXd dActive_dz = output.unaryExpr([&](double v) {return (*activationFunction)(v); });
	
	// dLoss_dz
	MatrixXd dLoss_doutput = lossFunction->diff(expectedValue_, output).cwiseProduct(dActive_dz);

	dLoss_dweights.block(0, 0, weights.rows(), weights.cols() - 1) = dLoss_doutput * input.transpose();
	dLoss_dweights.block(0, weights.cols() - 1, weights.rows(), 1) = dLoss_doutput.rowwise().sum();

	// previous_diff
	prev_diff = weights.block(0, 0, weights.rows(), weights.cols() - 1).transpose() * dLoss_doutput;

	return prev_diff;
}

double LastLayerBatchEfficient::loss(const MatrixXd& output_,const MatrixXd& expectedValue_)
{
	return (*lossFunction)(output_,expectedValue_);
}