#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork() :
	NeuralNetwork{10} {}

NeuralNetwork::NeuralNetwork(const int& maxNumLayers_) :
	maxNumLayers{ maxNumLayers_ }
{
	Layers.reserve(maxNumLayers);
}

void NeuralNetwork::addLayer(const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_ = ActivationType::RELU,
	const OptimizerType& optType_ = OptimizerType::SGD,
	const double& learningRate_)
{
	Layers.emplace_back(inputDim_, outputDim_, activType_, optType_, learningRate_);
	numLayers++;
}