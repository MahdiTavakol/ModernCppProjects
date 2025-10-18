#define CATCH_CONFIG_MAIN
#include "catch.hpp"


#include "testHelpers.h"

#define logger *loggerPtr

#include <memory>
#include <string>
#include <iostream>
#include <Eigen/Dense>

static int testNumber = 0;
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
	string extension = "-test-" + std::to_string(testNumber);
	nnMPIPtr = make_unique<NeuralNetworkMPI>(logger,
		setting.trainFileName, setting.testFileName,
		setting.nTargetCols, setting.nMaxLayers,
		setting.bSize, setting.maxNumSteps,0,extension);
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

	SUCCEED("Finished setting up");
}

TEST_CASE("Comparing the MPI result with SERIAL case for a 1 batch system")
{
	logger << endl << endl;
	logger << "Running test #" << testNumber++ << endl;
	SetupFiles(0);

	std::unique_ptr<NeuralNetwork> nnSerialPtr, nnMPIPtr;
	MatrixXd serialResult, mpiResult;

	logger << "Training the serial network" << endl;
	nnSerialPtr = make_unique<NeuralNetwork>(logger, 
		setting.trainFileName, setting.testFileName, 
		setting.nTargetCols, setting.nMaxLayers,
		setting.bSize, setting.maxNumSteps);
	nnSerialPtr->initializeInputFilePtr();
	nnSerialPtr->initializeOutputs();
	nnSerialPtr->initializeOptandLoss();
	nnSerialPtr->readInputFileData();
	nnSerialPtr->addLayer(setting.inDim, 4);
	nnSerialPtr->addLayer(4, 8);
	nnSerialPtr->addLayer(8, 4);
	nnSerialPtr->addLayer(4, setting.nTargetCols);
	nnSerialPtr->initializeLayers(seed);
	logger << "Transforming the serial network" << endl;
	nnSerialPtr->fit();
	serialResult = nnSerialPtr->transform();

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
	logger << "Transforming the MPI network" << endl;
	mpiResult = nnMPIPtr->transform();

	logger << "Comparing the results" << endl;
	Compare_Eigen_MatrixXds(serialResult, mpiResult);
	logger << endl << endl;
}


TEST_CASE("Comparing the MPI result for various batchsizes")
{
	logger << endl << endl;
	logger << "Running test #" << testNumber++ << endl;
	SetupFiles(0);
	vector<int> bSizes = { 2,4,8,16,32,64,128 };


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


	for (const int& bSize : bSizes)
	{
		logger << "Training the MPI network with batch size of " << bSize << endl;
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
		logger << "Transforming the network with the batch size of " << bSize << endl;
		mpiResult = nnMPIPtr->transform();

		logger << "Comparing results with the base network" << endl;
		Compare_Eigen_MatrixXds(mpiResult0, mpiResult);
	}
	logger << endl << endl;
}

TEST_CASE("Comparing the MPI result for various seed numbers")
{
	logger << endl << endl;
	logger << "Running test #" << testNumber++ << endl;
	SetupFiles(0);
	vector<int> seeds = { 1135,1245,2928,8769,8239,456,111 };


	std::unique_ptr<NeuralNetwork>  nnMPIPtr;
	MatrixXd mpiResult0, mpiResult;

	logger << "Training the base network with seed value of " << seed << endl;
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
	logger << "Transforming the base network with the seed of " << seed << endl;
	mpiResult0 = nnMPIPtr->transform();


	for (const int& sd : seeds)
	{
		logger << "Training the MPI network with batch size of " << setting.bSize << endl;
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
		nnMPIPtr->initializeLayers(sd);
		nnMPIPtr->fit();
		logger << "Transforming the network with the seed of " << sd << endl;
		mpiResult = nnMPIPtr->transform();

		logger << "Comparing results with the base network" << endl;
		Compare_Eigen_MatrixXds(mpiResult0, mpiResult);
	}
	logger << endl << endl;
}

TEST_CASE("Cleaning up!")
{
	logger << endl << endl;
	MPI_Finalize();
	SUCCEED("Finished cleaning up!");
	logger << endl << endl;
}

