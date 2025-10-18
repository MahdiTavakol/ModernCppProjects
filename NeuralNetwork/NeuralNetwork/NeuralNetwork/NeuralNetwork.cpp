#include "NeuralNetwork.h"
#include "Dropout.h"

#include <iostream>
#include <fstream>
#include <sstream>


NeuralNetwork::NeuralNetwork(Logger& logger_, const string& networkDataFileName_, const string& networkTestFileName_,
	const int& numTargetCols_, const int& maxNumLayers_,
	const int& batchsize_, const int& MaxNumSteps_, const string& fileNameExtension_) :
	logger{ logger_ },
	networkDataFileName{ networkDataFileName_ },
	networkTestFileName{ networkTestFileName_ },
	trainLossFileName{ "training-loss-" + fileNameExtension_ + ".dat" },
	validationLossFileName{ "validation-loss-" + fileNameExtension_ + ".dat" },
	numTargetCols{ numTargetCols_ }, 
	MaxNumSteps{MaxNumSteps_},
	batchsize{ batchsize_ },
	maxNumLayers{ maxNumLayers_ }
{}

void NeuralNetwork::initializeOptandLoss(const OptimizerType& optType_,
	                               const double& learningRate_,
	                               const LossType& lossType_)
{
	switch (optType_)
	{
	case OptimizerType::NONE:
		OptFuncPtr = std::make_unique<NoneOpt>(learningRate_);
		break;
	case OptimizerType::SGD:
		OptFuncPtr = std::make_unique<SGD>(learningRate_);
		break;
	case OptimizerType::SGDMOMENUM:
		OptFuncPtr = std::make_unique<SGDMomentum>(learningRate_);
		break;
	case OptimizerType::RMSPROP:
		OptFuncPtr = std::make_unique<RMSProp>(learningRate_);
		break;
	case OptimizerType::ADAM:
		OptFuncPtr = std::make_unique<Adam>(learningRate_);
		break;
	case OptimizerType::ADAGRAD:
		OptFuncPtr = std::make_unique<AdaGrad>(learningRate_);
		break;
	default:
		throw std::invalid_argument("Unknown optimizer type!");
	}


	switch (lossType_)
	{
	case LossType::MSE:
		LossFuncPtr = std::make_unique<MSE>();
		break;
	case LossType::MAE:
		LossFuncPtr = std::make_unique<MAE>();
		break;
	case LossType::Huber:
		LossFuncPtr = std::make_unique<Huber>();
		break;
	default:
		throw std::invalid_argument("Wrong loss function type");
	}

}

void NeuralNetwork::initializeInputFilePtr()
{
	InputFilePtr = std::make_unique<InputFile>(logger, networkDataFileName, numTargetCols);
	testPtr = std::make_unique<InputFile>(logger, networkTestFileName, 0);
}

void NeuralNetwork::initializeOutputs()
{
	if ((fOutputMode & AVG) || (fOutputMode & PERBATCH)) {
		if (logger.log_level >= LOG_LEVEL_INFO)
			std::cout << "Opening the files: " << trainLossFileName << " and " << validationLossFileName << std::endl; 
		
		// Check if the file name is initialized
		if (trainLossFileName.size()) {
			trainLossFile.open(trainLossFileName, std::ios::out | std::ios::trunc);
			if (!trainLossFile.is_open())
				logger << "Warning: Cannot open the traininglossfile" << std::endl;
		} else {
			logger << "Error: The trainLossFileName variable is not set!" << std::endl;
			throw std::runtime_error("Error: The trainLossFileName variable is not set!");
		}
		if (validationLossFileName.size()) {
			validationLossFile.open(validationLossFileName, std::ios::out | std::ios::trunc);
			if (!validationLossFile.is_open())
				logger << "Warning: Cannot open the validationlossfile" << std::endl;
		} else {
			logger << "Error: The trainValidationFileName variable is not set!" << std::endl;
			throw std::runtime_error("Error: The trainValidationFileName variable is not set!");
		}
	}
}

void NeuralNetwork::readInputFileData()
{
	if (InputFilePtr == nullptr) {
		std::cout << "InputFilePtr is empty, returning!" << std::endl;
		return;
	}
	InputFilePtr->init();
	InputFilePtr->read();
	InputFilePtr->return_data(networkInputFileDim, networkOutputDim, networkInputFileMatrix, networkOutputMatrix);
	InputFilePtr.reset();

	auto swap_array_2 = [](std::array<int, 2>& InputFile)
		{
			int dum = InputFile[0];
			InputFile[0] = InputFile[1];
			InputFile[1] = dum;
		};

	swap_array_2(networkInputFileDim);
	swap_array_2(networkOutputDim);
	networkInputFileMatrix.transposeInPlace();
	networkOutputMatrix.transposeInPlace();

	if (networkInputFileDim[1] != networkOutputDim[1]) {
		std::ostringstream oss;
		oss << "The number of data in the InputFile ("
			<< networkInputFileDim[1]
			<< ") and output data ("
			<< networkOutputDim[1]
			<< ") are not the same!";
		logger.print(oss.str());
		throw std::invalid_argument(oss.str());
	}


	// reserving the layers
	Layers.reserve(maxNumLayers);
}

void NeuralNetwork::addLayer(const int& InputFileDim_, const int& outputDim_,
	const ActivationType& activType_)
{
	// The first layer does not have any previous layer so we need to check if there is any previous layers
	if (Layers.size())
	{
		int prevLayerOutputDim = Layers.back()->returnInputFileOutputDims()[1];
		if (InputFileDim_ != prevLayerOutputDim) throw std::invalid_argument("Incompatible with the previous layer!");
	}
	if (batchsize == -1) {
		/* I do not put the default value of the batch size here
		* on purpose so that the Layers decides on the default value
		* avoiding having a mismatch between the two default values
		*/
		Layers.push_back(std::make_unique<LayerBatchEfficient>(logger, InputFileDim_, outputDim_,
			activType_));
	}
	else if (batchsize > 0) {
		Layers.push_back(std::make_unique<LayerBatchEfficient>(logger, batchsize, InputFileDim_, outputDim_,
			activType_));
	}
	else
	{
		logger.print("The batchsize must be > 0");
		throw std::invalid_argument("The batchsize must be > 0");
	}
	numLayers++;
}


void NeuralNetwork::addDropout(const int& dim_, const double& dropRate_)
{
	// checking if the size is the same is the previous layer
	if (Layers.size())
	{
		int prevLayerOutputDim = Layers.back()->returnInputFileOutputDims()[1];
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
		Layers.push_back(std::make_unique<Dropout>(logger, dim_,dropRate_));
	}
	else if (batchsize > 0) {
		Layers.push_back(std::make_unique<Dropout>(logger, batchsize, dim_, dropRate_));
	}
	else
	{
		logger.print("The batchsize must be > 0");
		throw std::invalid_argument("The batchsize must be > 0");
	}
	numLayers++;
}

void NeuralNetwork::initializeLayers(const int& seed_)
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
		if (seed_ == -1)
		{
			if (logger.log_level >= LOG_LEVEL_INFO)
				logger << "No random seed is provided: Using random_device as a seed value" << std::endl;
			layer->initialize();
		}
		else
			layer->initialize(seed_);
	}
}

void NeuralNetwork::fit()
{
	int numTData = static_cast<int>(trainingPercent * networkInputFileDim[1] / 100.0);
	int numVData = networkInputFileDim[1] - numTData;

	int numTBatchs = (numTData + batchsize - 1) / batchsize;
	int numVBatchs = (numVData + batchsize - 1) / batchsize;

	// Scaling the data
	scaler = std::make_unique<ZScoreScaler>();
	MatrixXd TDataXd = networkInputFileMatrix.block(0, 0, networkInputFileMatrix.rows(), numTData);
	scaler->fit(TDataXd);
	networkInputFileMatrix = (*scaler)(networkInputFileMatrix);
	networkOutputMatrix = (*scaler)(networkOutputMatrix);
	

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
		header_lambda(trainLossFile, numTBatchs);
		header_lambda(validationLossFile, numVBatchs);
	}

	int TDownVUp = 0;

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
		trainBatches(0, numTData, numTBatchs, tLossVal, true);
		tLossVal /= static_cast<double>(numTData);
		//tLossVal /= static_cast<double>(numTData*numFeatures);
		if (logger.log_level >= LOG_LEVEL_DEBUG)
			logger << "\tTraining loss: " << tLossVal << std::endl;

		if (logger.log_level >= LOG_LEVEL_DEBUG)
			logger << "\tTesting the data on the validating set" << std::endl;
		trainBatches(numTData, numVData, numVBatchs, vLossVal, false);
		vLossVal /= static_cast<double>(numVData);
		//vLossVal /= static_cast<double>(numVData*numFeatures);
		if (logger.log_level >= LOG_LEVEL_DEBUG)
			logger << "\tValidation loss: " << vLossVal << std::endl;

		if (logger.log_level >= LOG_LEVEL_DEBUG)
			logger << "\tAppending to the loss vectors and outputing values into files" << std::endl;

		//These two should be replaces with an instance of the Logger
		if (fOutputMode & AVG) {
			trainLossFile << "," << tLossVal << endl;
			validationLossFile << "," << vLossVal << endl;
		}

		double tLossPrev = trainingLoss.empty() ? std::numeric_limits<double>::infinity() : trainingLoss.back();
		double vLossPrev = validationLoss.empty() ? -std::numeric_limits<double>::infinity() : validationLoss.back();
		trainingLoss.push_back(tLossVal);
		validationLoss.push_back(vLossVal);
		if (tLossVal < trainingLoss.back() && vLossVal > validationLoss.back())
			TDownVUp++;
		else
			TDownVUp = 0;
		if (TDownVUp >= maxTDownVUp) {
			if (logger.log_level >= LOG_LEVEL_WARN)
				logger << "\tIn " << TDownVUp
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
	MatrixXd output;
	int numData;
	int numFeatures;

	testPtr->init();
	testPtr->read();
	testPtr->return_data(networkTestDim, networkDummyDim, networkTestMatrix, networkDummyMatrix);
	testPtr.reset();


	int dum = networkTestDim[0];
	networkTestDim[0] = networkTestDim[1];
	networkTestDim[1] = dum;

	networkTestMatrix.transposeInPlace();

	if (networkInputFileDim[0] != networkTestDim[0])
		throw std::invalid_argument("The number of feature for the test data is different than the training data!");

	numData = static_cast<int>(networkTestMatrix.cols());
	numFeatures = static_cast<int>(networkTestMatrix.rows());

	output.resize(numTargetCols, numData);

	int numBatchs = (numData + batchsize - 1) / batchsize;

	for (int j = 0; j < numBatchs; j++)
	{
		if (logger.log_level >= LOG_LEVEL_TRACE)
			logger << "\t\tWorking on the batch " << j << std::endl;
		MatrixXd outputBatch;

		int firstCol = batchsize * j;
		if (firstCol >= numData) break;
		int lastCol = std::min(firstCol + batchsize, numData);
		int numCols = lastCol - firstCol;

		MatrixXd TestFileBatch = networkTestMatrix.block(0, firstCol, networkTestMatrix.rows(), numCols);
		output.block(0,firstCol,output.rows(),numCols) = forwardBatch(TestFileBatch, false);
	}
	return output;
}

MatrixXd NeuralNetwork::forwardBatch(const MatrixXd& InputFile_, const bool& trainMode)
{
	MatrixXd outputMatrixBatch = InputFile_;
	int i = 0;
	if (logger.log_level >= LOG_LEVEL_TRACE)
		logger << "\t\tInputFile Matrix dims for forwardBatch = " << InputFile_.rows() << "X" << InputFile_.cols() << std::endl;
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

void NeuralNetwork::backwardBatch(const MatrixXd& lastDiff_)
{
	MatrixXd prevDiffBatch = lastDiff_;
	if (logger.log_level >= LOG_LEVEL_TRACE)
		logger << "\t\tInputFile Matrix dims for backwardBatch = " << lastDiff_.rows() << "X" << lastDiff_.cols() << std::endl;
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
		layer->update(OptFuncPtr);
		if (logger.log_level >= LOG_LEVEL_VERBOSE) {
			double testdLoss_deweights = layer->returnDLoss_deweights(0, 1);
			logger << "\t\t\tdLoss_dweights(0,1)=" << testdLoss_deweights << std::endl;
		}
	}
}

void NeuralNetwork::trainBatches(const int& firstData, const int& numData, const int& numBatchs, double& lossValue, const bool& doBack)
{
	lossValue = 0.0;
	MatrixXd outputBatch;
	
	for (int j = 0; j < numBatchs; j++)
	{
		if (logger.log_level >= LOG_LEVEL_TRACE)
			logger << "\t\tWorking on the batch " << j << std::endl;

		int firstCol = firstData + batchsize * j;
		if (firstCol-firstData >= numData) return;
		int lastCol = std::min(firstCol+batchsize,numData+firstData);
		int numCols = lastCol - firstCol;

		MatrixXd InputFileBatch = networkInputFileMatrix.block(0, firstCol, networkInputFileMatrix.rows(), numCols);
		MatrixXd expectedBatch = networkOutputMatrix.block(0, firstCol, networkOutputMatrix.rows(), numCols);

		if (logger.log_level >= LOG_LEVEL_TRACE) {
			logger << "\t\tInputFile Matrix dims = " << InputFileBatch.rows() << "X" << InputFileBatch.cols() << std::endl;
			logger << "\t\tBatch Cols = " << firstCol << " , " << lastCol << " and numData = " << numData << std::endl;
		}

		outputBatch = forwardBatch(InputFileBatch,doBack);
		if (doBack) {
			MatrixXd lastDiff = LossFuncPtr->diff(outputBatch,expectedBatch);
			backwardBatch(lastDiff);
			updateBatch();
		}


		double lossValueBatch = (*LossFuncPtr)(outputBatch, expectedBatch);
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

}

void NeuralNetwork::enterTestXy()
{
	networkInputFileDim[0] = 2;
	networkInputFileDim[1] = 4;
	networkOutputDim[0] = 1;
	networkOutputDim[1] = 4;
	networkInputFileMatrix.resize(2, 4);
	networkInputFileMatrix << 0, 1, 0, 1,
                      0, 0, 1, 1;

	networkOutputMatrix.resize(1, 4);
	networkOutputMatrix << 0, 1, 1, 0;
}

