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


	RunType runType = RunType::SERIAL;
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
	neuralNetworkPtr->readInputData();
	neuralNetworkPtr->addLayer(100, 50);
	neuralNetworkPtr->addLayer(50, 2);
	neuralNetworkPtr->addLastLayer(2, 1);
	neuralNetworkPtr->initializeLayers();
	neuralNetworkPtr->fit();
	neuralNetworkPtr->transform();

	
	MPI_Finalize();

	return 0;
}

