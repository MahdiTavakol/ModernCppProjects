#include "NeuralNetworkMPI.h"

NeuralNetworkMPI::NeuralNetworkMPI(const string networkInputFileName_, const string& networkOutputFileName_,
	const int& maxNumLayers_ = 10, const int& batchsize_ = -1):
	NeuralNetwork{networkInputFileName_,networkOutputFileName_, maxNumLayers_ , batchsize_}
{}

void trainBatches(const double& firstData, const double& numData, const int& numBatchs, double& lossValue, const bool& doBack)
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

			// Before this all the gradients from different ranks must be averaged!!!! add this please!!!
			updateBatch();
		}
		lossValue += lossBatch(outputBatch, expected);
	}
}