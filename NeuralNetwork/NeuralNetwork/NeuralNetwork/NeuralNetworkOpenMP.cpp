#include "NeuralNetworkOpenMP.h"

NeuralNetworkOpenMP::NeuralNetworkOpenMP(const string networkInputFileName_, const string& networkOutputFileName_,
	const int& maxNumLayers_ = 10, const int& batchsize_ = -1) :
	NeuralNetwork{ networkInputFileName_,networkOutputFileName_, maxNumLayers_ , batchsize_ }
{
#pragma omp single private(num_threads)
	{
		num_threads = omp_get_num_threads();
	}

	trainRangesLow.resize(num_threads);
	trainNums.resize(num_threads);
	validRangesLow.resize(num_threads);
	validNums.resize(num_threads);
}

void NeuralNetworkOpenMP::initializeData()
{
	inputPtr = std::make_unique<Input>(networkInputFileName, networkOutputFileName);
	inputPtr->read();
	inputPtr->return_data(networkInputDim, networkOutputDim, networkInputMatrix, networkOutputMatrix);
	inputPtr.reset();

	if (networkInputDim[1] != networkOutputDim[1])
		throw std::invalid_argument("The number of data in the input and output data are not the same!");
	Layers.reserve(maxNumLayers);
}

void NeuralNetworkOpenMP::train()
{
	int numTrainingData = static_cast<int>(trainingPercent * networkInputDim[1]);
	int numValidationData = networkInputDim[1] - numTrainingData;

	int numTrainingDataPerThread = numTrainingData / num_threads;
	int numValidationDataPerThread = numValidationData / num_threads;


	for (int i = 0; i < num_threads ; i++)
	{
		trainRangesLow[i] = i * numTrainingDataPerThread;
		int trainHigh = std::min(trainRangesLow[i] + numTrainingDataPerThread, numTrainingData);
		validRangesLow[i] = i * numValidationDataPerThread;
		int validHigh = std::min(validRangesLow[i] + numValidationDataPerThread, numValidationData);
		validRangesLow[i] += numTrainingData;
		validHigh += numTrainingData;

		int numT = trainHigh - trainRangesLow[i] + 1;
		int numV = validHigh - validRangesLow[i] + 1;
		trainNums[i] = numT;
		validNums[i] = numV;
	}


	int numStepsTrainLossDownValidLossUp = 0;

	for (int i = 0; i < MaxNumSteps; i++)
	{
		double tLoss = 0, vLoss = 0;

#pragma omp parallel reduction (+ : tLoss, vLoss)
		{
			std::array<double, 2> lossLocal;

			int thread_id = omp_get_thread_num();

			int numTrainingBatchs = (trainNums[thread_id] + batchsize - 1) / batchsize;
			int numValidationBatchs = (validNums[thread_id] + batchsize - 1) / batchsize;

			trainBatches(trainRangesLow[thread_id], trainNums[thread_id], numTrainingBatchs, lossLocal[0], true);
			trainBatches(validRangesLow[thread_id], validNums[thread_id], numValidationBatchs, lossLocal[1], false);
			
			tLoss += lossLocal[0];
			vLoss += lossLocal[1];
		}

		trainingLoss.push_back(tLoss);
		validationLoss.push_back(vLoss);

		if (tLoss < trainingLoss.back() && vLoss > validationLoss.back())
			numStepsTrainLossDownValidLossUp++;
		else
			numStepsTrainLossDownValidLossUp = 0;
		if (numStepsTrainLossDownValidLossUp >= 10)
			break;
	}
}

