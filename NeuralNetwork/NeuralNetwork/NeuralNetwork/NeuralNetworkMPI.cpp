#include "NeuralNetworkMPI.h"

NeuralNetworkMPI::NeuralNetworkMPI(Logger& logger_, const string& networkInputFileName_, const string& networkTestFileName_,
	const int& numTargetCols_,
	const int& maxNumLayers_, const int& batchsize_):
	NeuralNetwork{logger_, networkInputFileName_, networkTestFileName_, numTargetCols_, maxNumLayers_ , batchsize_}
{
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
		fOutputMode = AVG;
	else
		fOutputMode = 0;
		
	fOutputMode = AVG;
	fOutputMode |= PERRANK;
		
	trainLossFileName = "training-loss-rank-" + std::to_string(rank) + ".dat";
	validationLossFileName = "validation-loss-rank-" + std::to_string(rank) + ".dat";

}

void NeuralNetworkMPI::initializeInputPtr()
{
	inputPtr = std::make_unique<InputMPI>(logger,networkDataFileName,numTargetCols);
	testPtr = std::make_unique<InputMPI>(logger,networkTestFileName, 0);
}

void NeuralNetworkMPI::fit()
{
	int numTrainingData = static_cast<int>(trainingPercent * networkInputMatrix.cols() /100.0);
	int numValidationData = networkInputMatrix.cols() - numTrainingData;

	int numTrainingBatchs = (numTrainingData + batchsize - 1) / batchsize;
	int numValidationBatchs = (numValidationData + batchsize - 1) / batchsize;
	
	int numFeatures = networkInputMatrix.rows();

	
	trainingLoss.reserve(MaxNumSteps);
	validationLoss.reserve(MaxNumSteps);

	int numStepsTrainLossDownValidLossUp = 0;

	
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
	auto print_step = [&](ofstream& file, const int& i)
		{
			file << i;
		};
	if ((fOutputMode & AVG) || (fOutputMode & PERBATCH)) {
		header_lambda(trainLossFile, numTrainingBatchs);
		header_lambda(validationLossFile, numValidationBatchs);
	}


	for (int i = 0; i < MaxNumSteps; i++)
	{
		if ((fOutputMode & AVG) || (fOutputMode & PERBATCH)) {
			print_step(trainLossFile, i);
			print_step(validationLossFile, i);
		}
		
		if ((logger.log_level >= LOG_LEVEL_INFO) && (rank == 0))
			logger << "Step " << i << " out of " << MaxNumSteps << " steps " << std::endl;
		
		std::array<double, 2> lossLocal;
		std::array<double, 2> loss;
		int numDataLocal[2] = {numTrainingData,numValidationData};
		int numData[2] = {0,0};

		if (logger.log_level >= LOG_LEVEL_DEBUG && (rank == 0))
			logger << "\tTraining data on the training set" << std::endl;
		trainBatches(0, numTrainingData, numTrainingBatchs, lossLocal[0], true);
		if (logger.log_level >= LOG_LEVEL_DEBUG && (rank == 0))
			logger << "\tTesting the data on the validating set" << std::endl;
		trainBatches(numTrainingData, numValidationData, numValidationBatchs, lossLocal[1], false);

		MPI_Allreduce(lossLocal.data(), loss.data(), 2, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		MPI_Allreduce(numDataLocal,numData,2,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
		loss[0] /= static_cast<int>(numData[0]*numFeatures);
		loss[1] /= static_cast<int>(numData[1]*numFeatures);
		
		if (logger.log_level >= LOG_LEVEL_WARN) {
			if (numData[0] == 0 || numData[1] == 0 || numFeatures == 0)
				logger << "\tWarning: numData[0], numData[1], numFeatures == " 
				<< numData[0] << "," << numData[1] << "," << numFeatures << std::endl;
		}
 		
		if (logger.log_level >= LOG_LEVEL_DEBUG && (rank == 0))
			logger << "\tTraining loss: " << loss[0] << std::endl;
		if (logger.log_level >= LOG_LEVEL_DEBUG && (rank == 0))
			logger << "\tValidation loss: " << loss[1] << std::endl;

		trainingLoss.push_back(loss[0]);
		validationLoss.push_back(loss[1]);
		
		if (fOutputMode & AVG) {
			trainLossFile << "," << loss[0] << endl;
			validationLossFile << "," << loss[1] << endl;
		}

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
