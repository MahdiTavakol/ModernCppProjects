#pragma once

#include <vector>
#include <string>
#include <array>

#include "Activations.h"
#include "LayerBatchEfficient.h"
#include "Loss.h"
#include "InputFile.h"
#include "Logger.h"
#include "Scaler.h"

using std::vector;
using std::string;
using std::array;
using std::ofstream;
using std::cout;
using std::endl;

enum fileOutputMode {
	PERBATCH = 1<<0,
	AVG = 1<<1,
	PERRANK = 1<<2
};

enum class LossType
{
	MSE,
	MAE,
	Huber
};

enum class OptimizerType
{
	NONE,
	SGD,
	SGDMOMENUM,
	RMSPROP,
	ADAM,
	ADAGRAD
};


class NeuralNetwork
{
public:
	NeuralNetwork(Logger& logger_, const string& networkDataFileName_, const string& networkTestFileName_,
		const int& numTargetCols_, const int& maxNumLayers_ = 10,
		const int& batchsize_ = -1, const int& MaxNumSteps = 10000);
	void initializeOptandLoss(const OptimizerType& optType_ = OptimizerType::SGD,
		                const double& learningRate_ = 0.5, 
		                const LossType& lossType_ = LossType::MSE);
	virtual void initializeInputFilePtr();
	virtual void initializeOutputs();
	void readInputFileData();
	void addLayer(const int& InputFileDim_, const int& outputDim_,
		const ActivationType& activType_ = ActivationType::RELU);
	void addDropout(const int& dim_, const double& dropRate_);
	void initializeLayers(const int& seed_ = -1);
	virtual void fit();
	MatrixXd transform();
	void enterTestXy();

protected:
	// output parameters
	Logger& logger;
	ofstream trainLossFile, validationLossFile;
	std::string trainLossFileName, validationLossFileName;

	// Scaling function
	std::unique_ptr<Scaler> scaler;


	string networkDataFileName;
	string networkTestFileName;
	std::unique_ptr<InputFile> InputFilePtr;
	std::unique_ptr<InputFile> testPtr;
	const int numTargetCols;


	/*
	 * In the MPI version the networkInputFileMatrix dimensions is not the 
	 * same as the networkInputFileDim as the former is the local matrix
	 * and the latter is the dimension of the global matrix.
	 */
	array<int, 2> networkInputFileDim;
	array<int, 2> networkOutputDim;
	MatrixXd networkInputFileMatrix;
	MatrixXd networkOutputMatrix;

	// Control parameters
	const double trainingPercent = 70.0;
	const int MaxNumSteps;
	const int batchsize = -1;
	const int maxTDownVUp = 10;
	int numLayers = 0;
	int fOutputMode = AVG;

	int maxNumLayers;
	vector <std::unique_ptr<LayerBatchEfficient>> Layers;

	MatrixXd forwardBatch(const MatrixXd& InputFile_,const bool& trainMode);
	virtual void backwardBatch(const MatrixXd& expected_);
	void updateBatch();
	virtual void trainBatches(const int& firstData_, const int& numData_,
		const int& numBatchs_, double& lossValue_, const bool& doBack_);


	vector<double> trainingLoss;
	vector<double> validationLoss;

	std::unique_ptr<Optimizer> OptFuncPtr;
	std::unique_ptr<Loss> LossFuncPtr;
};
