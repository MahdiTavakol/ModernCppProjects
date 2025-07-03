#include "LastLayerBatchEfficient.h"

LastLayerBatchEfficient::LastLayerBatchEfficient(const int& inputDim_, const int& outputDim_, const ActivationType& activType_,
	const double& learningRate_, const LossType& lossType_):
	LayerBatchEfficient{inputDim_, outputDim_, activType_,learningRate_}, lossType{lossType_}
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


MatrixXd LastLayerBatchEfficient::backward(MatrixXd& expectedValue_)
{
	// dActivation_dz
	VectorXd z = weights * input + bias;
	VectorXd output = z.unaryExpr([&](double v) {return (*activationFunction)(v); });
	VectorXd dActive_dz = z.unaryExpr([&](double v) {return activationFunction->diff(v); });
	// dz_dweights
	RowVectorXd dz_dweights = input.transpose();
	// dLoss_dz
	VectorXd dLoss_dz = lossFunction->diff(expectedValue_, output).cwiseProduct(dActive_dz);

	dLoss_dweights = dLoss_dz * dz_dweights;

	// dzi_dbias
	VectorXd dz_dbias = Eigen::VectorXd::Ones(inputDim);
	dLoss_dbias = dLoss_dz * dz_dbias;

	// previous_diff
	prev_diff = weights.transpose() * dLoss_dz;

	return prev_diff;
}

double LastLayerBatchEfficient::loss(MatrixXd& output_, MatrixXd& expectedValue_)
{
	return (*lossFunction)(output_,expectedValue_);
}