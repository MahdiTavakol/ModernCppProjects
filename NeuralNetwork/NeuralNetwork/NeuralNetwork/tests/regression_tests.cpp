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

#include <memory>
#include <string>
#include <iostream>
#include <Eigen/Dense>

#define logger (*loggerPtr)

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
static constexpr int seed = 12345678;

static const struct Data {
	string trainFileName, testFileName;
	int nMaxLayers, nTargetCols, inDim, bSize;
} settings[] = {
	{ "../tests/train_1.csv", "../tests/test_1.csv", 4, 1, 2, 32 }
};
static Data setting;

static void Compare_Eigen_MatrixXds(const MatrixXd& a_, const MatrixXd& b_)
{
	// It the sizes are not the same comparison cannot be made.
	int aRows = static_cast<int>(a_.rows());
	int aCols = static_cast<int>(a_.cols());
	int bRows = static_cast<int>(b_.rows());
	int bCols = static_cast<int>(b_.cols());
	REQUIRE(aRows == bRows);
	REQUIRE(aCols == bCols);

	MatrixXd c = a_ - b_;
	double max_abs = c.cwiseAbs().maxCoeff();

	// Let's see if each element is the same!
	CHECKED_IF(max_abs <= eps)
	{
		SUCCEED("Test passed!");
	}
	CHECKED_ELSE(max_abs <= eps)
	{
		Eigen::Index i, j;
		c.cwiseAbs().maxCoeff(&i, &j);
		INFO("Max diff at (" << i << "," << j << ") "
			<< "a=" << a_(i, j) << " b=" << b_(i, j) << " diff=" << c(i, j));
	}
}

static void Setup()
{
	MPI_Init(NULL, NULL);
	vector<std::reference_wrapper<ostream>> logger_ref =
	{
		std::ref(std::cout)
	};
	loggerPtr = make_unique<Logger>( logger_ref, LOG_LEVEL_ERROR);
}

void SetupFiles(const int& indx)
{
	if (indx >= static_cast<int>(sizeof(settings) / sizeof(setting)))
		throw std::invalid_argument("No such a file");
	setting = settings[indx];
}

TEST_CASE("Setting up the global variables for tests")
{
	Setup();
	SUCCEED("Finished setting up");
}

TEST_CASE("Comparing the MPI result with SERIAL case for a 1 batch system")
{
	logger << "Running test #" << testNumber++ << endl;
	SetupFiles(0);

	std::unique_ptr<NeuralNetwork> nnSerialPtr, nnMPIPtr;
	MatrixXd serialResult, mpiResult;

	logger << "Training the serial network" << endl;
	nnSerialPtr = make_unique<NeuralNetwork>(logger, 
		setting.trainFileName, setting.testFileName, setting.nTargetCols, setting.nMaxLayers, setting.bSize);
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
		setting.trainFileName, setting.testFileName, setting.nTargetCols, setting.nMaxLayers, setting.bSize);
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
}


TEST_CASE("Comparing the MPI result for various batchsizes")
{
	logger << "Running test #" << testNumber++ << endl;
	SetupFiles(0);
	vector<int> bSizes = { 2,4,8,16,32,64,128 };


	std::unique_ptr<NeuralNetwork>  nnMPIPtr;
	MatrixXd mpiResult0, mpiResult;

	logger << "Training the base network with batch size of " << setting.bSize << endl;
	logger << "Training the MPI network" << endl;
	nnMPIPtr = make_unique<NeuralNetworkMPI>(logger,
		setting.trainFileName, setting.testFileName, setting.nTargetCols, setting.nMaxLayers, setting.bSize);
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
			setting.trainFileName, setting.testFileName, setting.nTargetCols, setting.nMaxLayers, bSize);
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
}

TEST_CASE("Comparing the MPI result for various seed numbers")
{
	logger << "Running test #" << testNumber++ << endl;
	SetupFiles(0);
	vector<int> seeds = { 1135,1245,2928,8769,8239,456,111 };


	std::unique_ptr<NeuralNetwork>  nnMPIPtr;
	MatrixXd mpiResult0, mpiResult;

	logger << "Training the base network with seed value of " << seed << endl;
	logger << "Training the MPI network" << endl;
	nnMPIPtr = make_unique<NeuralNetworkMPI>(logger,
		setting.trainFileName, setting.testFileName, setting.nTargetCols, setting.nMaxLayers, setting.bSize);
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
			setting.trainFileName, setting.testFileName, setting.nTargetCols, setting.nMaxLayers, setting.bSize);
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
}

TEST_CASE("Cleaning up!")
{
	MPI_Finalize();
	SUCCEED("Finished cleaning up!");
}

