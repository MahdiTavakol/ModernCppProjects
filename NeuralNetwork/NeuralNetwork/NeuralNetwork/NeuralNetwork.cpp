#include "NeuralNetwork.h"

#include <iostream>
#include <fstream>
#include <sstream>


NeuralNetwork::NeuralNetwork(const string networkInputFileName_, const string& networkOutputFileName_,
	const int& maxNumLayers_, const int& batchsize_) :
	networkInputFileName{networkInputFileName_},
	networkOutputFileName{ networkOutputFileName_},
	maxNumLayers{ maxNumLayers_ }, batchsize{batchsize_},
	trainingPercent{ 70.0 }
{}

void NeuralNetwork::initializeData()
{
	inputPtr = std::make_unique<Input>(networkInputFileName, networkOutputFileName);
	inputPtr->read();
	inputPtr->return_data(networkInputDim, networkOutputDim, networkInputMatrix, networkOutputMatrix);
	inputPtr.reset();

	if (networkInputDim[1] != networkOutputDim[1])
		throw std::invalid_argument("The number of data in the input and output data are not the same!");
	Layers.reserve(maxNumLayers);
}

void NeuralNetwork::addLayer(const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_ = ActivationType::RELU,
	const OptimizerType& optType_ = OptimizerType::SGD,
	const double& learningRate_)
{
	if (batchsize == -1) {
		/* I do not put the default value of the batch size here
		* on purpose so that the Layers decides on the default value
		* avoiding having a mismatch between the two default values
		*/
		Layers.push_back(std::make_unique<LayerBatchEfficient>(inputDim_, outputDim_,
			activType_, optType_, learningRate_));
	} else if (batchsize > 0) {
		Layers.push_back(std::make_unique<LayerBatchEfficient>(batchsize,inputDim_, outputDim_, 
			activType_, optType_, learningRate_));
	} else
		throw std::invalid_argument("The batchsize must be > 0");
	
	numLayers++;
}

void NeuralNetwork::addLastLayer(const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_ = ActivationType::RELU,
	const OptimizerType& optType_ = OptimizerType::SGD,
	const double& learningRate_)
{
	if (batchsize == -1) {
		Layers.push_back(std::make_unique<LastLayerBatchEfficient>(inputDim_, outputDim_,
			activType_, optType_, learningRate_));
	}
	else if (batchsize > 0) {
		Layers.push_back(std::make_unique<LastLayerBatchEfficient>(batchsize, inputDim_, outputDim_,
			activType_, optType_, learningRate_));
	}
	else
		throw std::invalid_argument("The batchsize must be > 0");
}

void NeuralNetwork::initializeLayers()
{
	// Checking the last layer type
	LayerBatchEfficient* LastLayerRawPtr = Layers.back().get();
	if (!(dynamic_cast<LastLayerBatchEfficient*>(LastLayerRawPtr)))
	{
		throw std::invalid_argument("The last layer must be of type LastLayerBatchEfficient!");
	}

	// Initializing the weights for each layer 
	for (auto& layer : Layers)
	{
		layer->initialize();
	}
}

void NeuralNetwork::train()
{
	int numTrainingData = static_cast<int>(trainingPercent * networkInputDim[1]);
	int numValidationData = networkInputDim[1] - numTrainingData;
	
	int numTrainingBatchs = (numTrainingData + batchsize - 1) / batchsize;
	int numValidationBatchs = (numValidationData + batchsize - 1) / batchsize;

	trainingLoss.reserve(MaxNumSteps);
	validationLoss.reserve(MaxNumSteps);

	int numStepsTrainLossDownValidLossUp = 0;

	for (int i = 0; i < MaxNumSteps; i++)
	{
		double trainingLossValue  = 0.0;
		double validationLossValue = 0.0;


		trainBatches(0, numTrainingData, numTrainingBatchs, trainingLossValue,true);
		trainBatches(numTrainingData, numValidationData, numValidationBatchs, validationLossValue,false);

		trainingLossValue /= static_cast<double> (numTrainingData);
		validationLossValue /= static_cast<double> (numValidationData);

		trainingLoss.push_back(trainingLossValue);
		validationLoss.push_back(validationLossValue);
		
		if (!trainingLoss.empty() && trainingLossValue < trainingLoss.back() && validationLossValue > validationLoss.back())
			numStepsTrainLossDownValidLossUp;
		else
			numStepsTrainLossDownValidLossUp = 0;
		if (numStepsTrainLossDownValidLossUp >= 10)
			break;

	}

}

MatrixXd NeuralNetwork::forwardBatch(const MatrixXd& input_)
{
	MatrixXd outputMatrixBatch = input_;
	for (auto& layer : Layers)
		outputMatrixBatch = layer->forward(outputMatrixBatch);
	return outputMatrixBatch;
}

void NeuralNetwork::backwardBatch(const MatrixXd& output_)
{
	MatrixXd prevDiffBatch = output_;
	for (int i = numLayers - 1; i >= 0; i--)
		prevDiffBatch = Layers[i]->backward(prevDiffBatch);
}

void NeuralNetwork::updateBatch()
{
	for (auto& layer : Layers)
		layer->update();
}

double NeuralNetwork::lossBatch(const MatrixXd& output_, const MatrixXd& expected_)
{
	return Layers.back()->loss(output_, expected_);
}

void NeuralNetwork::trainBatches(const double& firstData, const double& numData, const int& numBatchs, double& lossValue, const bool& doBack)
{
	for (int j = 0; j < numBatchs; j++)
	{
		MatrixXd outputBatch;

		int firstCol = batchsize * j;
		int lastCol = batchsize * (j + 1) < numData ? batchsize * (j + 1) : numData;
		firstCol += firstData;
		lastCol += firstData;
		int numCols = lastCol - firstCol + 1;

		MatrixXd input = networkInputMatrix.block(0, firstCol, networkInputMatrix.rows(), numCols);
		MatrixXd expected = networkOutputMatrix.block(0, firstCol, networkOutputMatrix.rows(), numCols);

		outputBatch = forwardBatch(input);
		if (doBack) {
			backwardBatch(outputBatch);
			updateBatch();
		}
		lossValue += lossBatch(outputBatch, expected);
	}
}
