#include "NeuralNetworkMPI.h"

NeuralNetworkMPI::NeuralNetworkMPI(const string networkInputFileName_, const string& networkOutputFileName_,
	const int& maxNumLayers_ = 10, const int& batchsize_ = -1):
	NeuralNetwork{networkInputFileName_,networkOutputFileName_, maxNumLayers_ , batchsize_}
{}

void NeuralNetworkMPI::backwardBatch(const MatrixXd& output_)
{
	MatrixXd prevDiffBatch = output_;
	for (int i = numLayers - 1; i >= 0; i--)
		prevDiffBatch = Layers[i]->backward(prevDiffBatch);
}

void NeuralNetworkMPI::trainBatches(const double& firstData, const double& numData,
	const int& numBatchs, double& lossValue, const bool& doBack)
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

void NeuralNetworkMPI::syncLayerGradients()
{
	for (auto& layer : Layers)
	{
		MatrixXd gradientLocal = layer->returnGradients();
		int rows = gradientLocal.rows();
		int cols = gradientLocal.cols();
		MatrixXd gradientAvg{ rows, cols };
		MPI_Allreduce(gradientLocal.data(), gradientAvg.data(), rows * cols*, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		gradientAvg.unaryExpr([&](double& v) {return v / size; });
		layer->updateGradients(std::move(gradientAvg));
	}

}