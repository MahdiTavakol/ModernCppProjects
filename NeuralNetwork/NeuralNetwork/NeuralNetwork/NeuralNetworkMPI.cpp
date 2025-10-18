#include "NeuralNetworkMPI.h"
#include <numeric>

NeuralNetworkMPI::NeuralNetworkMPI(Logger& logger_, const string& networkInputFileFileName_, const string& networkTestFileName_,
	                               const int& numTargetCols_, const int& maxNumLayers_, const int& batchsize_,
	                               const int& MaxNumSteps_, const int& exchangeNEvery_,
	                               const string& fileNameExtension_)  :
	NeuralNetwork{logger_, networkInputFileFileName_, networkTestFileName_,
	              numTargetCols_, maxNumLayers_ , batchsize_, MaxNumSteps_,fileNameExtension_},
	exchangeNEvery{exchangeNEvery_}
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
	}
}

void NeuralNetworkMPI::initializeInputFilePtr()
{
	InputFilePtr = std::make_unique<InputMPIFile>(logger,networkDataFileName,numTargetCols);
	testPtr = std::make_unique<InputMPIFile>(logger,networkTestFileName, 0);
}

void NeuralNetworkMPI::initializeOutputs()
{
	NeuralNetwork::initializeOutputs();
	if (rank == 0 && fOutputMode & AVG) {
		if (logger.log_level >= LOG_LEVEL_INFO)
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
	if (exchangeNEvery > 0) {
		setLastBatch();
	}
	

	int numTData = static_cast<int>(trainingPercent * networkInputFileMatrix.cols() /100.0);
	int numVData = static_cast<int>(networkInputFileMatrix.cols()) - numTData;

	int numTBatchs = (numTData + batchsize - 1) / batchsize;
	int numVBatchs = (numVData + batchsize - 1) / batchsize;
	
	int numFeatures = static_cast<int>(networkInputFileMatrix.rows());

	
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

		// Exchanging data between ranks.
		if (exchangeNEvery > 0 && !(i % exchangeNEvery))
			exchange();
		
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
		loss[0] /= static_cast<double>(numData[0]);
		loss[1] /= static_cast<double>(numData[1]);
		
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
			for (int rank_i = 0; rank_i < size; rank_i++) {
				trainLossFile << "," << lossTGather[rank_i]/static_cast<double>(nDataTGather[rank_i]);
				validationLossFile << "," << lossVGather[rank_i]/static_cast<double>(nDataVGather[rank_i]);
			}
			trainLossFile << std::endl;
			validationLossFile << std::endl;
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

void NeuralNetworkMPI::setLastBatch() {

	int status_0 = -1;

	if (rank != 0 && rank != size - 1) {
		return;
	} else if (rank == 0) {
		MPI_Status status;
		MPI_Recv(&status_0, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
	} else if (rank == size - 1) {
		int nCols;
		nCols = networkInputFileDim[1];
		status_0 = batchsize - nCols;
		MPI_Ssend(&status_0, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	if (rank == 0) {
		MatrixXd& inMat = networkInputFileMatrix;
		MatrixXd& ouMat = networkOutputMatrix;
		int nDataIn = static_cast<int>(inMat.rows()) * status_0;
		int nDataOu = static_cast<int>(ouMat.rows()) * status_0;
		MatrixXd tranferMatIn = inMat.block(0, 0, inMat.rows(), status_0);
		MatrixXd tranferMatOu = ouMat.block(0, 0, ouMat.rows(), status_0);
		MPI_Ssend(tranferMatIn.data(), nDataIn, MPI_DOUBLE, size - 1, 2, MPI_COMM_WORLD);
		MPI_Ssend(tranferMatOu.data(), nDataOu, MPI_DOUBLE, size - 1, 3, MPI_COMM_WORLD);
	} else if (rank == size - 1) {
		MPI_Status status[2];
		MatrixXd& inMat = networkInputFileMatrix;
		MatrixXd& ouMat = networkOutputMatrix;
		MatrixXd newIn{ inMat.rows(),inMat.cols() + status_0 };
		MatrixXd newOu{ ouMat.rows(),ouMat.cols() + status_0 };
		int nDataIn = static_cast<int>(inMat.rows()) * status_0;
		int nDataOu = static_cast<int>(ouMat.rows()) * status_0;
		MatrixXd transferMatIn{ inMat.rows(),status_0 };
		MatrixXd transferMatOu{ ouMat.rows(),status_0 };
		MPI_Recv(transferMatIn.data(), nDataIn, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status[0]);
		MPI_Recv(transferMatOu.data(), nDataOu, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD, &status[1]);
		newIn.block(0, 0, inMat.rows(), inMat.cols()) = inMat;
		newOu.block(0, 0, ouMat.rows(), ouMat.cols()) = ouMat;
		newIn.block(0, inMat.cols(), inMat.rows(), status_0) = transferMatIn;
		newOu.block(0, ouMat.cols(), ouMat.rows(), status_0) = transferMatOu;
		inMat = newIn;
		ouMat = newOu;
		networkInputFileDim[1] = static_cast<int>(inMat.cols());
		networkOutputDim[1] = static_cast<int>(ouMat.cols());
	}
	return;
}

MatrixXd NeuralNetworkMPI::transform()
{
	MatrixXd resultLocal = NeuralNetwork::transform();
	int numRowsLocal = static_cast<int>(resultLocal.rows());
	int numColsLocal = static_cast<int>(resultLocal.cols());
	int numDataLocal = numRowsLocal * numColsLocal;

	std::vector<int> recvCountVector;
	recvCountVector.resize(size);
	MPI_Allgather(&numDataLocal, 1, MPI_INT, recvCountVector.data(), 1, MPI_INT, MPI_COMM_WORLD);
	
	MatrixXd result;
	int numRows = numRowsLocal;
	int numData = std::accumulate(recvCountVector.begin(), recvCountVector.end(), 0);
	assert(numRows > 0);
	assert(numData % numRows == 0 && "Global elements not divisible by numRows");
	int numCols = numData / numRows;
	result.resize(numRows, numCols);

	std::vector<int> dispVector;
	dispVector.push_back(0);
	std::partial_sum(recvCountVector.begin(), recvCountVector.end()-1,std::back_inserter(dispVector));
	MPI_Allgatherv(resultLocal.data(), numDataLocal, MPI_DOUBLE,
		result.data(), recvCountVector.data(), dispVector.data(),MPI_DOUBLE,MPI_COMM_WORLD);


	return result;
}

void NeuralNetworkMPI::exchange()
{
	std::vector<int> sendRanks(size);
	// Since random numbers are involved the shuffle must be done on one rank and then transfered to other ranks.
	if (rank == 0) {
		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::iota(sendRanks.begin(), sendRanks.end(), 0);
		std::shuffle(sendRanks.begin(), sendRanks.end(), gen);
	}
	// inside the if it would cause a deadlock!
	MPI_Bcast(sendRanks.data(), size, MPI_INT, 0, MPI_COMM_WORLD);
	int recvRanki;
	int sendRanki;
	sendRanki = sendRanks[rank];
	auto it = std::find(sendRanks.begin(), sendRanks.end(), rank);
	if (it == sendRanks.end()) throw std::runtime_error("Something weird happened!");
	recvRanki = static_cast<int>(std::distance(sendRanks.begin(), it));

	// self transfer
	if (sendRanki == rank && recvRanki == rank) return;

	MPI_Request requests[4];
	MPI_Status statuses[4];
	const int numInputFileData = networkInputFileDim[0] * networkInputFileDim[1];
	MatrixXd newInputFileMatrix{networkInputFileMatrix.rows(),networkInputFileMatrix.cols() };
	MPI_Irecv(newInputFileMatrix.data(), numInputFileData, MPI_DOUBLE, recvRanki, rank, MPI_COMM_WORLD, &requests[0]);
	MPI_Isend(networkInputFileMatrix.data(), numInputFileData, MPI_DOUBLE, sendRanki, sendRanki, MPI_COMM_WORLD,&requests[1]);
	const int numOutputData = networkOutputDim[0] * networkOutputDim[1];
	MatrixXd newOutputMatrix{ networkOutputMatrix.rows(),networkOutputMatrix.cols() };
	MPI_Irecv(newOutputMatrix.data(), numOutputData, MPI_DOUBLE, recvRanki, 2*rank, MPI_COMM_WORLD,&requests[2]);
	MPI_Isend(networkOutputMatrix.data(), numOutputData, MPI_DOUBLE, sendRanki, 2*sendRanki, MPI_COMM_WORLD, &requests[3]);

	for (int i = 0; i < 4; i++)
		MPI_Wait(&requests[i], &statuses[i]);

	networkInputFileMatrix = newInputFileMatrix;
	networkOutputMatrix = newOutputMatrix;
}
