#pragma once

#include <vector>
#include <string>
#include <array>

#include "Activations.h"
#include "LayerBatchEfficient.h"
#include "LastLayerBatchEfficient.h"
#include "Loss.h"
#include "Input.h"
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

class NeuralNetwork
{
public:
	NeuralNetwork(Logger& logger_, const string& networkDataFileName_, const string& networkTestFileName_,
		const int& numTargetCols_, const int& maxNumLayers_ = 10,
		const int& batchsize_ = -1);
	virtual void initializeInputPtr();
	virtual void initializeOutputs();
	void readInputData();
	void addLayer(const int& inputDim_, const int& outputDim_,
		const ActivationType& activType_ = ActivationType::RELU,
		const OptimizerType& optType_ = OptimizerType::SGD,
		const double& learningRate_ = 0.1);
	void addLastLayer(const int& inputDim_, const int& outputDim_,
		const ActivationType& activType_ = ActivationType::SIGMOID,
		const OptimizerType& optType_ = OptimizerType::SGD,
		const double& learningRate_ = 0.05,
		const LossType& lossType_ = LossType::MSE);
	void addDropout(const int& dim_, const double& dropRate_, const double& learningRate_ = 0.1);
	void initializeLayers();
	virtual void fit();
	MatrixXd transform();

protected:
	// output parameters
	Logger& logger;
	ofstream trainLossFile, validationLossFile;
	std::string trainLossFileName, validationLossFileName;

	// Scaling function
	std::unique_ptr<Scaler> scaler;


	string networkDataFileName;
	string networkTestFileName;
	std::unique_ptr<Input> inputPtr;
	std::unique_ptr<Input> testPtr;
	const int numTargetCols;


	/*
	 * In the MPI version the networkInputMatrix dimensions is not the 
	 * same as the networkInputDim as the former is the local matrix
	 * and the latter is the dimension of the global matrix.
	 */
	array<int, 2> networkInputDim;
	array<int, 2> networkOutputDim;
	MatrixXd networkInputMatrix;
	MatrixXd networkOutputMatrix;

	// Control parameters
	const double trainingPercent = 70.0;
	const int MaxNumSteps = 10000;
	const int batchsize = -1;
	const int maxTDownVUp = 10;
	int numLayers = 0;

	int maxNumLayers;
	vector <std::unique_ptr<LayerBatchEfficient>> Layers;

	MatrixXd forwardBatch(const MatrixXd& input_,const bool& trainMode);
	virtual void backwardBatch(const MatrixXd& expected_);
	void updateBatch();


	virtual void trainBatches(const int& firstData_, const int& numData_,
		const int& numBatchs_, double& lossValue_, const bool& doBack_);


	vector<double> trainingLoss;
	vector<double> validationLoss;

	int fOutputMode = AVG;
};
