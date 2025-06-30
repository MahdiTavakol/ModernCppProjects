#include "LastLayerEfficient.h"

LastLayerEfficient::LastLayerEfficient(const int& inputDim_, const int& outputDim_, const ActivationType& activType_,
	const double& learningRate_, const LossType& lossType_):
	LayerEfficient{inputDim_, outputDim_, activType_,learningRate_}, lossType{lossType_}
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


void LastLayerEfficient::backward(VectorXd& expectedValue_)
{
	// dActivation_dz
	VectorXd output = weights * input + bias;
	VectorXd dActive_dz = output.unaryExpr([&](double v) {return activationFunction->diff(v); });
	// dzi_dweights
	MatrixXd dzi_dweights = input;
	// doutput_dwi
	MatrixXd doutput_dweights = dActive_dz * dzi_dweights;
	// dLoss_douput
	MatrixXd dLoss_doutput = lossFunction->diff(output, expectedValue_);
	// dLoss_dwi
	dLoss_dweights = dLoss_doutput * doutput_dweights;

	// dzi_dbias
	VectorXd dzi_dbias = Eigen::VectorXd::Ones(inputDim);
	VectorXd doutput_dbias = dActive_dz.cwiseProduct(dzi_dbias);
	dLoss_dbias = nextDiff_ * doutput_dbias;
}