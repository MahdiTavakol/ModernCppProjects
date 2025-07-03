#pragma once

#include <vector>
#include <string>
#include <array>

#include "Activations.h"
#include "LayerBatchEfficient.h"
#include "LastLayerBatchEfficient.h"
#include "Loss.h"

using std::vector;
using std::string;
using std::array;

class NeuralNetwork
{
public:
	NeuralNetwork(const string networkInputFileName_, const string& networkOutputFileName_, 
		const int& maxNumLayers_ = 10, const int& batchsize_ = -1);
	void addLayer(const int& inputDim_, const int& outputDim_,
		const ActivationType& activType_ = ActivationType::RELU,
		const OptimizerType& optType_ = OptimizerType::SGD,
		const double& learningRate_);
	void addLastLayer(const int& inputDim_, const int& outputDim_,
		const ActivationType& activType_ = ActivationType::RELU,
		const OptimizerType& optType_ = OptimizerType::SGD,
		const double& learningRate_);
	array<int,2> readCSVDataFile(string fileName_, MatrixXd& output_);
	void initialize();
	void train();



private:
	string networkInputFileName, networkOutputFileName;
	const array<int, 2> networkInputDim;
	const array<int, 2> networkOutputDim;
	MatrixXd networkInputMatrix;
	MatrixXd networkOutputMatrix;

	const double trainingPercent = 70.0;
	const int MaxNumSteps = 1000;
	const int batchsize = -1;
	int numLayers = 0;

	int maxNumLayers;
	vector <std::unique_ptr<LayerBatchEfficient>> Layers;

	MatrixXd forwardBatch(const MatrixXd& input_);
	void backwardBatch(const MatrixXd& output_);
	void updateBatch();
	double lossBatch(const MatrixXd& output_, const MatrixXd& expected_);


	vector<double> trainingLoss;
	vector<double> validationLoss;
};