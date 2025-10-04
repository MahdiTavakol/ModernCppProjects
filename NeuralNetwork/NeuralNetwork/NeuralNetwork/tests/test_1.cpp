#include <catch2/catch_test_macros.hpp>

#include "../Activations.h"
#include "../InputArgs.h"
#include "../InputFile.h"
#include "../InputMPIFile.h"
#include "../LayerBatchEfficient.h"
#include "../Loss.h"
#include "../Logger.h"
#include "../NeuralNetwork.h"
#include "../NeuralNetworkMPI.h"
#include "../NeuralNetworkOpenMP.h"
#include "../Optimizers.h"
#include "../Logger.h"

#include <memory>
#include <string>
#include <iostream>


TEST_CASE("1 Batch system with SERIAL")
{
	std::string trainFileName = "train-1.csv";
	std::string testFileName = "test-1.csv";

	std::vector<std::reference_wrapper<ostream>> logger_ref =
	{
		std::ref(std::cout)
	};
	Logger logger{ logger_ref };

	int nMaxLayers = 4;
	int nTargetCols = 1;
	int inDim = 2;
	int bSize = 32;

	logger.log_level >= LOG_LEVEL_ERROR;

	std::unique_ptr<NeuralNetwork> neuralNetworkPtr;
	NeuralNetworkPtr = std::make_unique<NeuralNetwork>(logger, trainFileName, testFileName, nTargetCols, nMaxLayers, bSize);


	neuralNetworkPtr->initializeInputFilePtr();
	neuralNetworkPtr->initializeOutputs();
	neuralNetworkPtr->initializeOptandLoss();
	neuralNetworkPtr->readInputFileData();
	neuralNetworkPtr->addLayer(2, 4);
	NeuralNetworkPtr->addLayer(4, 8);
	NeuralNetworkPtr->addLayer(8, 4);
	NeuralNetworkPtr->addLayer(4, 1);
	neuralNetworkPtr->initializeLayers();
	neuralNetworkPtr->fit();
	neuralNetworkPtr->transform();
}

