#pragma once

#include "Activations.h"
#include "LayerEfficient.h"
#include "LastLayerEfficient.h"
#include "Loss.h"


class NeuralNetwork
{
public:
	NeuralNetwork();
	void addLayer();
	void run();


private:
	int numLayers;
	std::unique_ptr<LayerEfficient[]> layers;

	void forward();
	void backward();
	void update();
};