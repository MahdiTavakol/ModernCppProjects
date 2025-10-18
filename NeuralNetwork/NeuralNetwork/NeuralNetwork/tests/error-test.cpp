#define CATCH_CONFIG_MAIN
#include "catch.hpp"


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
#include "testHelpers.h"

#include <memory>
#include <string>
#include <iostream>
#include <Eigen/Dense>



using std::ostream;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using Eigen::MatrixXd;

static int testNumber = 0;
static constexpr double eps = 1e-8;
static unique_ptr<Logger> loggerPtr;
auto& logger = *loggerPtr;
static constexpr int seed = 12345678;

TEST_CASE("Setting up the global variables for tests")
{
	Setup();
	SUCCEED("Finished setting up");
	logger << endl << endl;
}

TEST_CASE("Testing the convergence of neural network MPI for a simple data")
{
	logger << endl << endl;
	logger << "Running test #" << testNumber++ << endl;
	SetupFiles(0);
	int bSize = 32;


	std::unique_ptr<NeuralNetwork>  nnMPIPtr;
	MatrixXd mpiResult0, mpiResult;

	logger << "Training the base network with batch size of " << setting.bSize << endl;
	logger << "Training the MPI network" << endl;
	nnMPIPtr = make_unique<NeuralNetworkMPI>(logger,
		setting.trainFileName, setting.testFileName,
		setting.nTargetCols, setting.nMaxLayers,
		setting.bSize, setting.maxNumSteps);
	nnMPIPtr->initializeInputFilePtr();
	nnMPIPtr->initializeOutputs();
	nnMPIPtr->initializeOptandLoss();
	nnMPIPtr->readInputFileData();
	nnMPIPtr->addLayer(setting.inDim, 4);
	nnMPIPtr->addLayer(4, 8);
	nnMPIPtr->addLayer(8, 4);
	nnMPIPtr->addLayer(4, setting.nTargetCols);
	nnMPIPtr->initializeLayers(seed);
	nnMPIPtr->fit();
	logger << "Transforming the base network with the batch size of " << setting.bSize << endl;
	mpiResult0 = nnMPIPtr->transform();

	logger << endl << endl;
}