#pragma once
#include "NeuralNetwork.h"
#include "InputMPI.h"

#include <mpi.h>

class NeuralNetworkMPI final : public NeuralNetwork
{
public:
	NeuralNetworkMPI(const string& networkInputFileName_, const string& networkTestFileName_, 
		const int& numTargetCols_,
		const int& maxNumLayers_ = 10, const int& batchsize_ = -1);
	void initializeInputPtr() override;
	void fit() override;

protected:
	void backwardBatch(const MatrixXd& output_) override;


private:
	int rank, size;
};