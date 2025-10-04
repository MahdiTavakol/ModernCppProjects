#include "NeuralNetworkOpenMP.h"

NeuralNetworkOpenMP::NeuralNetworkOpenMP(Logger& logger_, const string& networkInputFileFileName_, const string& networkTestFileName_,
	const int& numTargetCols_,
	const int& maxNumLayers_, const int& batchsize_) :
	NeuralNetwork{logger_, networkInputFileFileName_, networkTestFileName_,numTargetCols_,maxNumLayers_ , batchsize_ }
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


void NeuralNetworkOpenMP::fit()
{
	int numTData = static_cast<int>(trainingPercent * networkInputFileDim[1]);
	int numVData = networkInputFileDim[1] - numTData;


	int numTBatchs = (numTData + batchsize - 1) / batchsize;
	int numVBatchs = (numVData + batchsize - 1) / batchsize;

	trainingLoss.reserve(MaxNumSteps);
	validationLoss.reserve(MaxNumSteps);

	int TDownVUp = 0;


	for (int i = 0; i < MaxNumSteps; i++)
	{
		double tLossVal = 0.0;
		double vLossVal = 0.0;


		trainBatches(0, numTData, numTBatchs, tLossVal, true);
		trainBatches(numTData, numVData, numVBatchs, vLossVal, false);

		trainingLoss.push_back(tLossVal);
		validationLoss.push_back(vLossVal);

		if (!trainingLoss.empty() && tLossVal < trainingLoss.back() && vLossVal > validationLoss.back())
			TDownVUp++;
		else
			TDownVUp = 0;
		if (TDownVUp >= maxTDownVUp)
			break;

	}
}

void NeuralNetworkOpenMP::trainBatches(const int& firstData_, const int& numData_,
	const int& numBatchs_, double& lossValue_, const bool& doBack_)
{
	double lossVal = 0.0;

	vector<vector<MatrixXd>> gradientAvgThreads;
	gradientAvgThreads.resize(num_threads);

	for (int j = 0; j < num_threads; j++)
		gradientAvgThreads[j].resize(numLayers);

	for (int i = numLayers - 1; i >= 0  && doBack_ ; i--)
	{
		auto& layer = Layers[i];
		array<int, 2> dim = layer->returnGradientSize();
		int rows = dim[0];
		int cols = dim[1];
		for (int j = 0; j < num_threads; j++)
			gradientAvgThreads[j][i] = MatrixXd::Zero(rows, cols);
	}

#pragma omp parallel reduction( + : lossVal) 
	{
		int thread_id = omp_get_thread_num();
		for (int j = 0; j < numBatchs_; j++)
		{
			MatrixXd outputBatch;

			int nums = trainNums[thread_id];
			int init = trainRangesLow[thread_id];
			if (doBack_)
			{
				nums = validNums[thread_id];
				init = validRangesLow[thread_id];
			}

			int firstCol = batchsize * j;
			int lastCol = batchsize * (j + 1) < nums ? batchsize * (j + 1) : nums;
			firstCol += init;
			lastCol += init;
			int numCols = lastCol - firstCol + 1;


			MatrixXd InputFile = networkInputFileMatrix.block(0, firstCol, networkInputFileMatrix.rows(), numCols);
			MatrixXd expected = networkOutputMatrix.block(0, firstCol, networkOutputMatrix.rows(), numCols);

			outputBatch = forwardBatch(InputFile,doBack_);
			if (doBack_) {
				MatrixXd prevDiffBatch = outputBatch;
				for (int i = numLayers - 1; i >= 0; i--)
				{
					auto& layer = Layers[i];
					prevDiffBatch = layer->backward(prevDiffBatch);
					gradientAvgThreads[thread_id][i] += layer->returnGradients();
				}
				for (auto& layer : Layers)
					layer->update();
			}
			//lossVal += lossBatch(outputBatch, expected);
		}
	}

	if (doBack_)
	{
		for (int i = numLayers - 1; i >= 0; i--)
		{
			auto rows = gradientAvgThreads[0][i].rows();
			auto cols = gradientAvgThreads[0][i].cols();
			MatrixXd gradientAvg = MatrixXd::Zero( rows,cols );
			for (int j = 0; j < num_threads; j++)
				gradientAvg += gradientAvgThreads[j][i];
			gradientAvg = gradientAvg.unaryExpr([&](double v) {return v / num_threads; });
			Layers[i]->updateGradients(std::move(gradientAvg));
		}
	}

	lossValue_ = lossVal;
}
