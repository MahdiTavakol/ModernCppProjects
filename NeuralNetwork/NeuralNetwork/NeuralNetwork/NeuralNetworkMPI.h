#pragma once
#include "NeuralNetwork.h"
#include "InputMPIFile.h"
#include "Logger.h"

#include <mpi.h>

class NeuralNetworkMPI final : public NeuralNetwork
{
public:
	NeuralNetworkMPI(Logger& logger_, const string& networkInputFileFileName_, const string& networkTestFileName_, 
		const int& numTargetCols_,
		const int& maxNumLayers_ = 10, const int& batchsize_ = -1, const int& MaxNumSteps = 1000, const int& exchangeNEvery_=0);
	void initializeInputFilePtr() override;
	void initializeOutputs() override;
	void fit() override;

protected:
	void backwardBatch(const MatrixXd& output_) override;


private:
	int rank, size;

	std::string TVLossAllFileName;
	std::ofstream TVLossAllFile;

	int exchangeNEvery;
	void setLastBatch();
	void exchange();
};
