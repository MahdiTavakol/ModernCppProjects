#pragma once

#include <vector>

#include "Activations.h"
#include "LayerEfficient.h"
#include "LastLayerEfficient.h"
#include "Loss.h"

using std::vector;

class NeuralNetwork
{
public:
	NeuralNetwork();
	NeuralNetwork(const int& maxNumLayers_);
	void addLayer(const int& inputDim_, const int& outputDim_, 
		const ActivationType& activType_ = ActivationType::RELU, 
		const OptimizerType& optType_ = OptimizerType::SGD,
		const double& learningRate_);
	void run();


private:
	int numLayers = 0;

	int maxNumLayers;
	vector<LayerEfficient> Layers;

	void forward();
	void backward();
	void update();
};