#include "NeuralNetworkMPI.h"

NeuralNetworkMPI::NeuralNetworkMPI(const string networkInputFileName_, const string& networkOutputFileName_,
	const int& maxNumLayers_ = 10, const int& batchsize_ = -1):
	NeuralNetwork{networkInputFileName_,networkOutputFileName_, maxNumLayers_ , batchsize_}
{
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
}

void NeuralNetworkMPI::initializeData()
{
	inputPtr = std::make_unique<InputMPI>(networkInputFileName, networkOutputFileName);
	inputPtr->read();
	inputPtr->return_data(networkInputDim, networkOutputDim, networkInputMatrix, networkOutputMatrix);
	inputPtr.reset();

	if (networkInputDim[1] != networkOutputDim[1])
		throw std::invalid_argument("The number of data in the input and output data are not the same!");
	Layers.reserve(maxNumLayers);
}

void NeuralNetworkMPI::backwardBatch(const MatrixXd& output_)
{
	MatrixXd prevDiffBatch = output_;
	for (int i = numLayers - 1; i >= 0; i--)
	{
		auto& layer = Layers[i];
		prevDiffBatch = layer->backward(prevDiffBatch);
		MatrixXd gradientLocal = layer->returnGradients();
		int rows = gradientLocal.rows();
		int cols = gradientLocal.cols();
		MatrixXd gradientAvg{ rows, cols };
		MPI_Allreduce(gradientLocal.data(), gradientAvg.data(), rows * cols, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		gradientAvg.unaryExpr([&](double& v) {return v / size; });
		layer->updateGradients(std::move(gradientAvg));
	}
}
