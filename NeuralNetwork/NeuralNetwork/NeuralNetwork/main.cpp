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
#include <iostream>

enum class RunType
{
	SERIAL,
	MPI,
	OPENMP
};


int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	std::string trainFile = "data/train.csv";
	std::string testFile = "data/test.csv";

	RunType runType = RunType::MPI;
	std::unique_ptr<NeuralNetwork> neuralNetworkPtr;
	
	switch (runType)
	{
	case RunType::SERIAL:
		// trainfiles, n_in (input_cols) , n_out
		neuralNetworkPtr = std::make_unique<NeuralNetwork>(trainFile,testFile,1,13,32);
		break;
	case RunType::MPI:
		neuralNetworkPtr = std::make_unique<NeuralNetworkMPI>(trainFile, testFile, 1,13, 32);
		break;
	case RunType::OPENMP:
		neuralNetworkPtr = std::make_unique<NeuralNetworkOpenMP>(trainFile, testFile,1, 13, 32);
		break;
	default:
		throw std::invalid_argument("Uknown run type");
	}

	neuralNetworkPtr->initializeInputPtr();
	neuralNetworkPtr->readInputData();
	neuralNetworkPtr->addLayer(100, 90);
	neuralNetworkPtr->addLayer(90, 80);
	neuralNetworkPtr->addLayer(80, 70);
	neuralNetworkPtr->addLayer(70, 60);
	neuralNetworkPtr->addLayer(60, 50);
	neuralNetworkPtr->addLayer(50, 40);
	neuralNetworkPtr->addLayer(40, 30);
	neuralNetworkPtr->addLayer(30, 20);
	neuralNetworkPtr->addLayer(20, 10);
	neuralNetworkPtr->addLayer(10, 6);
	neuralNetworkPtr->addLayer(6, 4);
	neuralNetworkPtr->addLayer(4, 2);
	neuralNetworkPtr->addLastLayer(2, 1);
	neuralNetworkPtr->initializeLayers();
	neuralNetworkPtr->fit();
	neuralNetworkPtr->transform();

	
	MPI_Finalize();

	return 0;
}

