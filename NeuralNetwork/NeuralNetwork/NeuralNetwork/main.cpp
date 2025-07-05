#include "Activations.h"
#include "Input.h"
#include "InputMPI.h"
#include "LastLayerBatchEfficient.h"
#include "LayerBatchEfficient.h"
#include "Loss.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkMPI.h"
#include "NeuralNetworkOpenMP.h"
#include "Optimizers.h"

#include <memory>
#include <string>

enum class RunType
{
	SERIAL,
	MPI,
	OPENMP
};

void main()
{
	std::string inputFileName = "input.dat";
	std::string outputFileName = "output.dat";

	RunType runType = RunType::MPI;
	std::unique_ptr<NeuralNetwork> neuralNetworkPtr;
	
	switch (runType)
	{
	case RunType::SERIAL:
		neuralNetworkPtr = std::make_unique<NeuralNetwork>(inputFileName, outputFileName, 5, 32);
		break;
	case RunType::MPI:
		neuralNetworkPtr = std::make_unique<NeuralNetworkMPI>(inputFileName, outputFileName, 5, 32);
		break;
	case RunType::OPENMP:
		neuralNetworkPtr = std::make_unique<NeuralNetworkOpenMP>(inputFileName, outputFileName, 5, 32);
		break;
	default:
		throw std::invalid_argument("Uknown run type");

	}

	neuralNetworkPtr->initializeData();
	neuralNetworkPtr->addLayer(10, 8);
	neuralNetworkPtr->addLayer(8, 5);
	neuralNetworkPtr->addLayer(5, 3);
	neuralNetworkPtr->addLastLayer(3, 3);

	neuralNetworkPtr->initializeLayers();
	neuralNetworkPtr->train();
}