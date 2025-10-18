#pragma once

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

static std::unique_ptr<Logger> loggerPtr;
auto& logger = *loggerPtr;

static const struct Data {
	string trainFileName, testFileName;
	int nMaxLayers, nTargetCols, inDim, bSize;
	int maxNumSteps;
} settings[] = {
	{ "../tests/train_1.csv", "../tests/test_1.csv", 4, 1, 2, 32,1000}
};
static Data setting;

static void Compare_Eigen_MatrixXds(const MatrixXd& a_, const MatrixXd& b_)
{
	// It the sizes are not the same comparison cannot be made.
	int aRows = static_cast<int>(a_.rows());
	int aCols = static_cast<int>(a_.cols());
	int bRows = static_cast<int>(b_.rows());
	int bCols = static_cast<int>(b_.cols());
	logger << aCols << "," << aRows << "," << bCols << "," << bRows << endl;
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
	loggerPtr = make_unique<Logger>(logger_ref, LOG_LEVEL_ERROR);
}

void SetupFiles(const int& indx)
{
	if (indx >= static_cast<int>(sizeof(settings) / sizeof(setting)))
		throw std::invalid_argument("No such a file");
	setting = settings[indx];
}