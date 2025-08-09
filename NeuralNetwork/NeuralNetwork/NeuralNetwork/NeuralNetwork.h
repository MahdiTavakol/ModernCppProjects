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

using std::vector;
using std::string;
using std::array;
using std::ofstream;
using std::cout;
using std::endl;

class NeuralNetwork
{
public:
	NeuralNetwork(Logger& logger_, const string& networkDataFileName_, const string& networkTestFileName_,
		const int& numTargetCols_, const int& maxNumLayers_ = 10,
		const int& batchsize_ = -1);
	virtual void initializeInputPtr();
	void readInputData();
	void addLayer(const int& inputDim_, const int& outputDim_,
		const ActivationType& activType_ = ActivationType::RELU,
		const OptimizerType& optType_ = OptimizerType::SGD,
		const double& learningRate_ = 0.1);
	void addLastLayer(const int& inputDim_, const int& outputDim_,
		const ActivationType& activType_ = ActivationType::RELU,
		const OptimizerType& optType_ = OptimizerType::SGD,
		const double& learningRate_ = 0.1,
		const LossType& lossType_ = LossType::MSE);
	void initializeLayers();
	virtual void fit();
	MatrixXd transform();

protected:
	// output parameters
	Logger& logger;
	ofstream trainLossFile, validationLossFile;


	string networkDataFileName;
	string networkTestFileName;
	std::unique_ptr<Input> inputPtr;
	std::unique_ptr<Input> testPtr;
	const int numTargetCols;


	array<int, 2> networkInputDim;
	array<int, 2> networkOutputDim;
	MatrixXd networkInputMatrix;
	MatrixXd networkOutputMatrix;

	const double trainingPercent = 70.0;
	const int MaxNumSteps = 10; // 1000;
	const int batchsize = -1;
	int numLayers = 0;

	int maxNumLayers;
	vector <std::unique_ptr<LayerBatchEfficient>> Layers;

	MatrixXd forwardBatch(const MatrixXd& input_);
	virtual void backwardBatch(const MatrixXd& output_);
	void updateBatch();
	double lossBatch(const MatrixXd& output_, const MatrixXd& expected_);


	virtual void trainBatches(const int& firstData_, const int& numData_,
		const int& numBatchs_, double& lossValue_, const bool& doBack_);


	vector<double> trainingLoss;
	vector<double> validationLoss;


};