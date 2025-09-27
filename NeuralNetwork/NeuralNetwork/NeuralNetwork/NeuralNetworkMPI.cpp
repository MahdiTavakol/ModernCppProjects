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
		

	if (rank == 0) {
		trainLossFileName = "training-loss-perRank.dat";
		validationLossFileName = "validation-loss-perRank.dat";
		TVLossAllFileName = "TVLosses.dat";
		TVLossAllFile.open(TVLossAllFileName, std::ios::out | std::ios::trunc);
		if (!TVLossAllFile.is_open())
			logger << "Warning: Cannot open the RVLossAllFile" << std::endl;
	}

}

void NeuralNetworkMPI::initializeInputPtr()
{
	inputPtr = std::make_unique<InputMPI>(logger,networkDataFileName,numTargetCols);
	testPtr = std::make_unique<InputMPI>(logger,networkTestFileName, 0);
}

void NeuralNetworkMPI::initializeOutputs()
{
	NeuralNetwork::initializeOutputs();
	if (rank == 0 && fOutputMode & AVG) {
		std::cout << "Opening the file: " << TVLossAllFileName  << std::endl;

		// Check if the file name is initialized
		if (TVLossAllFileName.size()) {
			TVLossAllFile.open(TVLossAllFileName, std::ios::out | std::ios::trunc);
			if (!TVLossAllFile.is_open())
				logger << "Warning: Cannot open the file " << TVLossAllFileName << std::endl;
		}
	}
}

void NeuralNetworkMPI::fit()
{
	int numTData = static_cast<int>(trainingPercent * networkInputMatrix.cols() /100.0);
	int numVData = static_cast<int>(networkInputMatrix.cols()) - numTData;

	int numTBatchs = (numTData + batchsize - 1) / batchsize;
	int numVBatchs = (numVData + batchsize - 1) / batchsize;
	
	int numFeatures = static_cast<int>(networkInputMatrix.rows());

	
	trainingLoss.reserve(MaxNumSteps);
	validationLoss.reserve(MaxNumSteps);

	// Close these files on other ranks.. We use just one MPI_Gather and then write all the data in one file.....

	// The number of steps in which the training loss reduces while the validation loss increases (sign of overfitting)
	int TDownVUp = 0;

	
	auto header_lambda = [&](ofstream& file, const int& numBatchs)
		{
			file << "Step";
			if (fOutputMode & PERBATCH) {
				for (int i = 0; i < numBatchs; i++)
					file << ",Batch-" << i;
				file << ",All-batches";
			}
			if (fOutputMode & PERRANK) {
				for (int i = 0; i < size; i++)
					file << ",Rank-" << i;
			}
			
			file << std::endl;
		};
	auto print_step = [&](ofstream& file, const int& i)
		{
			file << i;
		};
	if (rank == 0) {
		if ((fOutputMode & AVG) || (fOutputMode & PERBATCH) || (fOutputMode & PERRANK)) {
			header_lambda(trainLossFile, numTBatchs);
			header_lambda(validationLossFile, numVBatchs);
		}
	}


	for (int i = 0; i < MaxNumSteps; i++)
	{
		if ((fOutputMode & AVG) || (fOutputMode & PERBATCH)) {
			print_step(trainLossFile, i);
			print_step(validationLossFile, i);
		}
		
		if ((logger.log_level >= LOG_LEVEL_INFO) && (rank == 0))
			logger << "Step " << i << " out of " << MaxNumSteps << " steps " << std::endl;
		
		std::array<double, 2> lossLocal = { 0.0,0.0 };
		std::array<double, 2> loss = { 0.0, 0.0 };

		std::vector<double> lossTGather;
		std::vector<double> lossVGather;
		std::vector<int> nDataTGather;
		std::vector<int> nDataVGather;
		// resize puts zero into the vector --> no need to push_back despite the reserve
		lossTGather.resize(size);
		lossVGather.resize(size);
		nDataTGather.resize(size);
		nDataVGather.resize(size);

		int numDataLocal[2] = {numTData,numVData};
		int numData[2] = {0,0};

		if (logger.log_level >= LOG_LEVEL_DEBUG && (rank == 0))
			logger << "\tTraining data on the training set" << std::endl;
		trainBatches(0, numTData, numTBatchs, lossLocal[0], true);
		if (logger.log_level >= LOG_LEVEL_DEBUG && (rank == 0))
			logger << "\tTesting the data on the validating set" << std::endl;
		trainBatches(numTData, numVData, numVBatchs, lossLocal[1], false);

		MPI_Allreduce(lossLocal.data(), loss.data(), 2, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		MPI_Allreduce(numDataLocal,numData,2,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
		MPI_Gather(&lossLocal[0], 1, MPI_DOUBLE, lossTGather.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Gather(&lossLocal[1], 1, MPI_DOUBLE, lossVGather.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Gather(&numDataLocal[0], 1, MPI_INT, nDataTGather.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Gather(&numDataLocal[1], 1, MPI_INT, nDataVGather.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
		loss[0] /= static_cast<double>(numData[0]*numFeatures);
		loss[1] /= static_cast<double>(numData[1]*numFeatures);
		
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
		
		if (rank == 0 && fOutputMode & AVG) {
			for (int i = 0; i < size; i++) {
				trainLossFile << "," << lossTGather[i]/static_cast<double>(nDataTGather[i] * numFeatures);
				validationLossFile << "," << lossVGather[i]/static_cast<double>(nDataVGather[i] * numFeatures);
			}
			TVLossAllFile << loss[0] << "," << loss[1] << std::endl;
		}

		// some double checking we should never reach here!!!
		int stop_one = 0, stop_all;
		// We have averaged the loss over all the ranks with MPI_Allreduce so these vectors are the same in all the ranks
		if (!trainingLoss.empty() && loss[0] < trainingLoss.back() && loss[1] > validationLoss.back())
			TDownVUp++;
		else
			TDownVUp = 0;
		if (TDownVUp >= maxTDownVUp)
			stop_one = 1;

		// Checking that if all the ranks stop to avoid deadlock-> it should always happen 
		MPI_Allreduce(&stop_one, &stop_all, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
		if (stop_all != size * stop_one)
			throw std::runtime_error("You should never have reached here!");
		if (stop_one) {
			if (logger.log_level >= LOG_LEVEL_WARN && rank == 0)
				logger << "\tIn " << TDownVUp
				<< " steps the training loss reduced while the validation loss increased!"
				<< std::endl << " Stopping the iteration!" << std::endl;
			break;
		}

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
		int rows = static_cast<int>(gradientLocal.rows());
		int cols = static_cast<int>(gradientLocal.cols());
		MatrixXd gradientAvg{ rows, cols };
		MPI_Allreduce(gradientLocal.data(), gradientAvg.data(), rows * cols, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		gradientAvg = gradientAvg.unaryExpr([&](double v) {return v / size; });
		layer->updateGradients(std::move(gradientAvg));
	}
}
