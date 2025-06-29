#include "Neuron.h"


Neuron::Neuron(const ActivationType& activType_):
	activType{activType_}
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
	}
}

void Neuron::initialize(const VectorXd& weights_, const VectorXd& input_)
{
	weights = weights_;
	input = input_;
}

double Neuron::forward()
{
	return weights.dot(input);
}