#include "NeuralNetwork.h"
#include "Dropout.h"

#include <iostream>
#include <fstream>
#include <sstream>


NeuralNetwork::NeuralNetwork(Logger& logger_, const string& networkDataFileName_, const string& networkTestFileName_,
	const int& numTargetCols_, const int& maxNumLayers_,
	const int& batchsize_) :
	logger{ logger_ },
	trainLossFileName{"training-loss.dat"},
	validationLossFileName{"validation-loss.dat"},
	networkDataFileName{ networkDataFileName_ },
	networkTestFileName{ networkTestFileName_ },
	numTargetCols{ numTargetCols_ },
	maxNumLayers{ maxNumLayers_ }, batchsize{ batchsize_ },
	trainingPercent{ 70.0 }
{}

void NeuralNetwork::initializeInputPtr()
{
	inputPtr = std::make_unique<Input>(logger, networkDataFileName, numTargetCols);
	testPtr = std::make_unique<Input>(logger, networkTestFileName, 0);
}

void NeuralNetwork::initializeOutputs()
{
	if ((fOutputMode & AVG) || (fOutputMode & PERBATCH)) {
		std::cout << "Opening the files: " << trainLossFileName << " and " << validationLossFileName << std::endl; 
		
		// Check if the file name is initialized
		if (trainLossFileName.size()) {
			trainLossFile.open(trainLossFileName, std::ios::out | std::ios::trunc);
			if (!trainLossFile.is_open())
				logger << "Warning: Cannot open the traininglossfile" << std::endl;
		}
		if (validationLossFileName.size()) {
			validationLossFile.open(validationLossFileName, std::ios::out | std::ios::trunc);
			if (!validationLossFile.is_open())
				logger << "Warning: Cannot open the validationlossfile" << std::endl;
		}
	}
}

void NeuralNetwork::readInputData()
{
	if (inputPtr == nullptr) {
		std::cout << "InputPtr is empty, returning!" << std::endl;
		return;
	}
	inputPtr->init();
	inputPtr->read();
	inputPtr->return_data(networkInputDim, networkOutputDim, networkInputMatrix, networkOutputMatrix);
	inputPtr.reset();

	auto swap_array_2 = [](std::array<int, 2>& input)
		{
			int dum = input[0];
			input[0] = input[1];
			input[1] = dum;
		};

	swap_array_2(networkInputDim);
	swap_array_2(networkOutputDim);
	networkInputMatrix.transposeInPlace();
	networkOutputMatrix.transposeInPlace();

	if (networkInputDim[1] != networkOutputDim[1]) {
		std::ostringstream oss;
		oss << "The number of data in the input ("
			<< networkInputDim[1]
			<< ") and output data ("
			<< networkOutputDim[1]
			<< ") are not the same!";
		logger.print(oss.str());
		throw std::invalid_argument(oss.str());
	}

	// Scaling the data
	scaler = std::make_unique<ZScoreScaler>();
	networkInputMatrix = (*scaler)(networkInputMatrix);
	//networkOutputMatrix = (*scaler)(networkOutputMatrix);

	// reserving the layers
	Layers.reserve(maxNumLayers);
}

void NeuralNetwork::addLayer(const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_,
	const OptimizerType& optType_,
	const double& learningRate_)
{
	// The first layer does not have any previous layer so we need to check if there is any previous layers
	if (Layers.size())
	{
		int prevLayerOutputDim = Layers.back()->returnInputOutputDims()[1];
		if (inputDim_ != prevLayerOutputDim) throw std::invalid_argument("Incompatible with the previous layer!");
	}
	if (batchsize == -1) {
		/* I do not put the default value of the batch size here
		* on purpose so that the Layers decides on the default value
		* avoiding having a mismatch between the two default values
		*/
		Layers.push_back(std::make_unique<LayerBatchEfficient>(logger, inputDim_, outputDim_,
			activType_, optType_, learningRate_));
	}
	else if (batchsize > 0) {
		Layers.push_back(std::make_unique<LayerBatchEfficient>(logger, batchsize, inputDim_, outputDim_,
			activType_, optType_, learningRate_));
	}
	else
	{
		logger.print("The batchsize must be > 0");
		throw std::invalid_argument("The batchsize must be > 0");
	}
	numLayers++;
}

void NeuralNetwork::addLastLayer(const int& inputDim_, const int& outputDim_,
	const ActivationType& activType_,
	const OptimizerType& optType_,
	const double& learningRate_,
	const LossType& lossType_)
{
	// It might be the only layer
	if (Layers.size())
	{
		int prevLayerOutputDim = Layers.back()->returnInputOutputDims()[1];
		if (inputDim_ != prevLayerOutputDim) throw std::invalid_argument("Incompatible with the previous layer!");
	}
	if (batchsize == -1) {
		/* I do not put the default value of the batch size here
		 * on purpose so that the Layers decides on the default value
		 * avoiding having a mismatch between the two default values
		 */
		Layers.push_back(std::make_unique<LastLayerBatchEfficient>(logger, inputDim_, outputDim_,
			activType_, optType_, learningRate_, lossType_));
	}
	else if (batchsize > 0) {
		Layers.push_back(std::make_unique<LastLayerBatchEfficient>(logger, batchsize, inputDim_, outputDim_,
			activType_, optType_, learningRate_, lossType_));
	}
	else
		throw std::invalid_argument("The batchsize must be > 0");
	numLayers++;
}

void NeuralNetwork::addDropout(const int& dim_, const double& dropRate_, const double& learningRate_)
{
	// checking if the size is the same is the previous layer
	if (Layers.size())
	{
		int prevLayerOutputDim = Layers.back()->returnInputOutputDims()[1];
		if (dim_ != prevLayerOutputDim) throw std::invalid_argument("Incompatible with the previous layer!");
	}
	// The first layer cannot be a dropout as there is no previous weights to drop out!
	if (!Layers.size())
	{
		throw std::invalid_argument("The first layer cannot be a dropout!");
	}
	if (batchsize == -1) {
		/* I do not put the default value of the batch size here
		* on purpose so that the Layers decides on the default value
		* avoiding having a mismatch between the two default values
		*/
		Layers.push_back(std::make_unique<Dropout>(logger, dim_, learningRate_, dropRate_));
	}
	else if (batchsize > 0) {
		Layers.push_back(std::make_unique<Dropout>(logger, batchsize, dim_, learningRate_, dropRate_));
	}
	else
	{
		logger.print("The batchsize must be > 0");
		throw std::invalid_argument("The batchsize must be > 0");
	}
	numLayers++;
}

void NeuralNetwork::initializeLayers()
{
	// Checking the last layer type
	//LayerBatchEfficient* LastLayerRawPtr = Layers.back().get();
	//if (!(dynamic_cast<LastLayerBatchEfficient*>(LastLayerRawPtr)))
	//{
	//	throw std::invalid_argument("The last layer must be of type LastLayerBatchEfficient!");
	//}

	if (logger.log_level >= LOG_LEVEL_INFO)
		logger << "Initializing layers" << std::endl;
	// Initializing the weights for each layer 
	int layerNumber = 0;
	for (auto& layer : Layers)
	{
		if (logger.log_level >= LOG_LEVEL_DEBUG)
			logger << "Initializing the layer " << ++layerNumber << std::endl;
		layer->initialize();
	}
}

void NeuralNetwork::fit()
{
	int numTData = static_cast<int>(trainingPercent * networkInputDim[1] / 100.0);
	int numVData = networkInputDim[1] - numTData;

	int numTrainingBatchs = (numTData + batchsize - 1) / batchsize;
	int numValidationBatchs = (numVData + batchsize - 1) / batchsize;
	
	int numFeatures = static_cast<int>(networkInputMatrix.rows());

	trainingLoss.reserve(MaxNumSteps);
	validationLoss.reserve(MaxNumSteps);
	

	auto header_lambda = [&](ofstream& file, const int& numBatchs)
		{
			file << "Step";
			if (fOutputMode & PERBATCH) {
				for (int i = 0; i < numBatchs; i++)
					file << ",Batch-" << i;
			}
			file << ",All-batches";
			file << std::endl;
		};
	auto print_step = [&](ofstream& file, const int& stepNumber)
		{
			file << stepNumber;
		};
	if ((fOutputMode & AVG) || (fOutputMode & PERBATCH)) {
		header_lambda(trainLossFile, numTrainingBatchs);
		header_lambda(validationLossFile, numValidationBatchs);
	}

	int numStepsTrainLossDownValidLossUp = 0;

	for (int i = 0; i < MaxNumSteps; i++)
	{
		if ((fOutputMode & AVG) || (fOutputMode & PERBATCH)) {
			print_step(trainLossFile, i);
			print_step(validationLossFile, i);
		}
		if (logger.log_level >= LOG_LEVEL_INFO)
			logger << "Step " << i << " out of " << MaxNumSteps << " steps " << std::endl;
		double tLossVal = 0.0;
		double vLossVal = 0.0;

		if (logger.log_level >= LOG_LEVEL_DEBUG)
			logger << "\tTraining data on the training set" << std::endl;
		trainBatches(0, numTData, numTrainingBatchs, tLossVal, true);
		tLossVal /= static_cast<double>(numTData);
		//tLossVal /= static_cast<double>(numTData*numFeatures);
		if (logger.log_level >= LOG_LEVEL_DEBUG)
			logger << "\tTraining loss: " << tLossVal << std::endl;

		if (logger.log_level >= LOG_LEVEL_DEBUG)
			logger << "\tTesting the data on the validating set" << std::endl;
		trainBatches(numTData, numVData, numValidationBatchs, vLossVal, false);
		vLossVal /= static_cast<double>(numVData);
		//vLossVal /= static_cast<double>(numVData*numFeatures);
		if (logger.log_level >= LOG_LEVEL_DEBUG)
			logger << "\tValidation loss: " << vLossVal << std::endl;

		if (logger.log_level >= LOG_LEVEL_DEBUG)
			logger << "\tAppending to the loss vectors and outputing values into files" << std::endl;
		trainingLoss.push_back(tLossVal);
		validationLoss.push_back(vLossVal);
		//These two should be replaces with an instance of the Logger
		if (fOutputMode & AVG) {
			trainLossFile << "," << tLossVal << endl;
			validationLossFile << "," << vLossVal << endl;
		}

		if (!trainingLoss.empty() && tLossVal < trainingLoss.back() && vLossVal > validationLoss.back())
			numStepsTrainLossDownValidLossUp++;
		else
			numStepsTrainLossDownValidLossUp = 0;
		if (numStepsTrainLossDownValidLossUp >= maxTDownVUp) {
			if (logger.log_level >= LOG_LEVEL_WARN)
				logger << "\tIn " << numStepsTrainLossDownValidLossUp 
				<< " steps the training loss reduced while the validation loss increased!" 
				<< std::endl << " Stopping the iteration!" << std::endl;
			break;
		}
	}
}

MatrixXd NeuralNetwork::transform()
{
	array<int, 2> networkTestDim;
	array<int, 2> networkDummyDim;
	MatrixXd networkTestMatrix;
	MatrixXd networkDummyMatrix;

	testPtr->init();
	testPtr->read();
	testPtr->return_data(networkTestDim, networkDummyDim, networkTestMatrix, networkDummyMatrix);
	testPtr.reset();


	int dum = networkTestDim[0];
	networkTestDim[0] = networkTestDim[1];
	networkTestDim[1] = dum;

	networkTestMatrix.transposeInPlace();

	if (networkInputDim[0] != networkTestDim[0])
		throw std::invalid_argument("The number of feature for the test data is different than the training data!");

	return forwardBatch(networkTestMatrix,false);
}

MatrixXd NeuralNetwork::forwardBatch(const MatrixXd& input_, const bool& trainMode)
{
	MatrixXd outputMatrixBatch = input_;
	int i = 0;
	if (logger.log_level >= LOG_LEVEL_TRACE)
		logger << "\t\tInput Matrix dims for forwardBatch = " << input_.rows() << "X" << input_.cols() << std::endl;
	for (auto& layer : Layers)
	{
		if (logger.log_level >= LOG_LEVEL_VERBOSE)
			logger << "\t\t\tForward on the layer " << i++ << std::endl;
		outputMatrixBatch = layer->forward(outputMatrixBatch,trainMode);
		if (logger.log_level >= LOG_LEVEL_VERBOSE)
			logger << "\t\t\tOutputMatrixBatch dims = " << outputMatrixBatch.rows() << "X" << outputMatrixBatch.cols() << std::endl;
	}
	return outputMatrixBatch;
}

void NeuralNetwork::backwardBatch(const MatrixXd& expected_)
{
	MatrixXd prevDiffBatch = expected_;
	if (logger.log_level >= LOG_LEVEL_TRACE)
		logger << "\t\tInput Matrix dims for backwardBatch = " << expected_.rows() << "X" << expected_.cols() << std::endl;
	for (int i = numLayers - 1; i >= 0; i--)
	{
		if (logger.log_level >= LOG_LEVEL_VERBOSE)
			logger << "\t\t\tBackward on the layer " << i << std::endl;
		prevDiffBatch = Layers[i]->backward(prevDiffBatch);
		if (logger.log_level >= LOG_LEVEL_VERBOSE)
			logger << "\t\t\tprevDiffBatch dims = " << prevDiffBatch.rows() << "X" << prevDiffBatch.cols() << std::endl;
	}
}

void NeuralNetwork::updateBatch()
{
	if (logger.log_level >= LOG_LEVEL_TRACE)
		logger << "\t\tUpdating batches " << std::endl;
	int i = 0;
	for (auto& layer : Layers)
	{
		if (logger.log_level >= LOG_LEVEL_VERBOSE)
			logger << "\t\t\tUpdating the layer " << i++ << std::endl;
		layer->update();
		if (logger.log_level >= LOG_LEVEL_VERBOSE) {
			double testdLoss_deweights = layer->returnDLoss_deweights(0, 1);
			logger << "\t\t\tdLoss_dweights(0,1)=" << testdLoss_deweights << std::endl;
		}
	}
}

void NeuralNetwork::trainBatches(const int& firstData, const int& numData, const int& numBatchs, double& lossValue, const bool& doBack)
{
	lossValue = 0.0;
	
	LastLayerBatchEfficient* LastLayerRawPtr = dynamic_cast<LastLayerBatchEfficient*> (Layers.back().get());
	if (!LastLayerRawPtr) throw std::runtime_error("The last layer type is wrong!");
	
	for (int j = 0; j < numBatchs; j++)
	{
		if (logger.log_level >= LOG_LEVEL_TRACE)
			logger << "\t\tWorking on the batch " << j << std::endl;
		MatrixXd outputBatch;

		int firstCol = firstData + batchsize * j;
		if (firstCol-firstData >= numData) return;
		int lastCol = std::min(firstCol+batchsize,numData+firstData);
		int numCols = lastCol - firstCol;

		MatrixXd inputBatch = networkInputMatrix.block(0, firstCol, networkInputMatrix.rows(), numCols);
		MatrixXd expectedBatch = networkOutputMatrix.block(0, firstCol, networkOutputMatrix.rows(), numCols);

		if (logger.log_level >= LOG_LEVEL_TRACE) {
			logger << "\t\tInput Matrix dims = " << inputBatch.rows() << "X" << inputBatch.cols() << std::endl;
			logger << "\t\tBatch Cols = " << firstCol << " , " << lastCol << " and numData = " << numData << std::endl;
		}

		outputBatch = forwardBatch(inputBatch,doBack);
		if (doBack) {
			backwardBatch(expectedBatch);
			updateBatch();
		}


		double lossValueBatch = LastLayerRawPtr->loss(outputBatch, expectedBatch);
		lossValue += lossValueBatch*numCols;


		if (logger.log_level >= LOG_LEVEL_TRACE)
			logger << "\t\tOutput Matrix dims = " << outputBatch.rows() << "X" << outputBatch.cols() << std::endl;
		if (logger.log_level >= LOG_LEVEL_VERBOSE)
		{
			if (doBack)
				logger << "\t\tTrainLossbatch = ";
			else
				logger << "\t\tValidationLossbatch = ";
			logger << lossValueBatch << std::endl;
		}
		if (fOutputMode & PERBATCH) {
			if (doBack)
				trainLossFile << "," << lossValueBatch;
			else
				validationLossFile << "," << lossValueBatch;
		}
	}
	
	array<int,2> lastDims = LastLayerRawPtr->returnInputOutputDims();
}

void NeuralNetwork::enterTestXy()
{
	networkInputDim[0] = 2;
	networkInputDim[1] = 4;
	networkOutputDim[0] = 1;
	networkOutputDim[1] = 4;
	networkInputMatrix.resize(2, 4);
	networkInputMatrix << 0, 1, 0, 1,
                      0, 0, 1, 1;

	networkOutputMatrix.resize(1, 4);
	networkOutputMatrix << 0, 1, 1, 0;
}

