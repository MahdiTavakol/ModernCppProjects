#include "NeuralNetworkMPI.h"

NeuralNetworkMPI::NeuralNetworkMPI(const string networkInputFileName_, const string& networkOutputFileName_,
	const int& maxNumLayers_, const int& batchsize_):
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

void NeuralNetworkMPI::train()
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
		std::array<double, 2> lossLocal;
		std::array<double, 2> loss;


		trainBatches(0, numTrainingData, numTrainingBatchs, lossLocal[0], true);
		trainBatches(numTrainingData, numValidationData, numValidationBatchs, lossLocal[1], false);

		MPI_Allreduce(lossLocal.data(), loss.data(), 2, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

		trainingLoss.push_back(loss[0]);
		validationLoss.push_back(loss[1]);

		if (!trainingLoss.empty() && loss[0] < trainingLoss.back() && loss[1] > validationLoss.back())
			numStepsTrainLossDownValidLossUp;
		else
			numStepsTrainLossDownValidLossUp = 0;
		if (numStepsTrainLossDownValidLossUp >= 10)
			break;

	}
}

void NeuralNetworkMPI::backwardBatch(const MatrixXd& output_)
{
	MatrixXd prevDiffBatch = output_;
	for (int i = numLayers - 1; i >= 0; i--)
	{
		auto& layer = Layers[i];
		prevDiffBatch = layer->backward(prevDiffBatch);
		MatrixXd gradientLocal = layer->moveGradients();
		int rows = gradientLocal.rows();
		int cols = gradientLocal.cols();
		MatrixXd gradientAvg{ rows, cols };
		MPI_Allreduce(gradientLocal.data(), gradientAvg.data(), rows * cols, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		gradientAvg = gradientAvg.unaryExpr([&](double v) {return v / size; });
		layer->updateGradients(std::move(gradientAvg));
	}
}
