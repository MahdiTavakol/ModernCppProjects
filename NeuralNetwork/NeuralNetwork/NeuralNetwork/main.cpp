#include "Activations.h"
#include "Input.h"
#include "InputMPI.h"
#include "LastLayerBatchEfficient.h"
#include "LayerBatchEfficient.h"
#include "Loss.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkMPI.h"
#include "Optimizers.h"

#include <memory>
#include <string>

void main()
{
	std::string inputFileName = "input.dat";
	std::string outputFileName = "output.dat";
	std::unique_ptr<NeuralNetwork> neuralNetworkPtr{std::make_unique<NeuralNetwork>(inputFileName, outputFileName, 5, 32)};

	neuralNetworkPtr->initializeData();
	neuralNetworkPtr->addLayer(10, 8);
	neuralNetworkPtr->addLayer(8, 5);
	neuralNetworkPtr->addLayer(5, 3);
	neuralNetworkPtr->addLastLayer(3, 3);

	neuralNetworkPtr->initializeLayers();
	neuralNetworkPtr->train();
}