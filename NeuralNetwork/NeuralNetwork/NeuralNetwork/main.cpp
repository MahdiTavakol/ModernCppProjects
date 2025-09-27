#include "Activations.h"
#include "Input.h"
#include "InputMPI.h"
#include "LastLayerBatchEfficient.h"
#include "LayerBatchEfficient.h"
#include "Loss.h"
#include "Logger.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkMPI.h"
#include "NeuralNetworkOpenMP.h"
#include "Optimizers.h"
#include "Logger.h"

#include <memory>
#include <string>
#include <iostream>

using std::ostream;


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
	std::ofstream logFile{ "log.txt" };
	std::ofstream outFile1{ "Losses.dat" };
	std::ofstream outFile2{ "BatchLosses.dat" };
	std::vector<std::reference_wrapper<ostream>> logger_ref =
	{
		std::ref(std::cout),
		std::ref(logFile)
	};
	Logger logger{logger_ref};


	RunType runType = RunType::MPI;
	std::unique_ptr<NeuralNetwork> neuralNetworkPtr;

	if (logger.log_level >= LOG_LEVEL_INFO)
	{
		logger << "Initializing the program" << std::endl;
		logger << "Creating the Neural Network" << std::endl;
	}
	
	switch (runType)
	{
	case RunType::SERIAL:
		// trainfiles, n_in (input_cols) , n_out
		neuralNetworkPtr = std::make_unique<NeuralNetwork>(logger,trainFile,testFile,1,13,32);
		break;
	case RunType::MPI:
		neuralNetworkPtr = std::make_unique<NeuralNetworkMPI>(logger,trainFile, testFile, 1,13, 32);
		break;
	case RunType::OPENMP:
		neuralNetworkPtr = std::make_unique<NeuralNetworkOpenMP>(logger,trainFile, testFile,1, 13, 32);
		break;
	default:
		throw std::invalid_argument("Uknown run type");
	}

	neuralNetworkPtr->initializeInputPtr();
	neuralNetworkPtr->initializeOutputs();
	neuralNetworkPtr->readInputData();
	neuralNetworkPtr->addLayer(100, 90);
	neuralNetworkPtr->addDropout(90,0.1);
	neuralNetworkPtr->addLayer(90,  80);
	neuralNetworkPtr->addDropout(80,0.1);
	neuralNetworkPtr->addLayer(80,  70);
	neuralNetworkPtr->addDropout(70,0.1);
	neuralNetworkPtr->addLayer(70,  50);
	neuralNetworkPtr->addDropout(50,0.1);
	neuralNetworkPtr->addLayer(50, 30);
	neuralNetworkPtr->addDropout(30,0.1);
	neuralNetworkPtr->addLayer(30, 10);
	neuralNetworkPtr->addDropout(10,0.1);
	neuralNetworkPtr->addLayer(10, 2);
	neuralNetworkPtr->addDropout(2,0.1);
	neuralNetworkPtr->addLastLayer(2, 1);
	neuralNetworkPtr->initializeLayers();
	neuralNetworkPtr->fit();
	//neuralNetworkPtr->transform();

	
	MPI_Finalize();

	return 0;
}

