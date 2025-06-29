#include "Neuron.h"


Neuron::Neuron(const ActivationType& activType_, const double& learningRate_):
	activType{activType_}, learningRate{learningRate_}
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

void Neuron::reset(const VectorXd& weights_, const VectorXd& input_)
{
	weights = weights_;
	input = input_;
}

double Neuron::forward()
{
	return (*activationFunction)(weights.dot(input) + bias);
}

void Neuron::backward(const double& diffPrevious)
{
	// dActivation_dz
	double dActive_dz = activationFunction->diff(weights.dot(input) + bias);
	// dzi_dweights
	VectorXd dzi_dweights = input;
	// doutput_dwi
	VectorXd doutput_dweights = dActive_dz * dzi_dweights;
	// dLoss_dwi
	dLoss_dweights = diffPrevious * doutput_dweights;

	// dzi_dbias
	double dzi_dbias = 1.0;
	double doutput_dbias = dActive_dz * dzi_dbias;
	dLoss_dbias = diffPrevious * doutput_dbias;
}

void Neuron::update()
{
	weights -= learningRate * dLoss_dweights;
	bias -= learningRate * dLoss_dbias;
}