#pragma once
#include "NeuralNetwork.h"

#include <omp.h>

class NeuralNetworkOpenMP final : public NeuralNetwork
{
public:
	NeuralNetworkOpenMP(Logger& logger_, const string& networkInputFileName_, const string& networkTestFileName_, 
		const int& numTargetCols_,
		const int& maxNumLayers_ = 10, const int& batchsize_ = -1);
	void fit() override;

protected:
	void backwardBatch(const MatrixXd& /*output_*/) {}

	void trainBatches(const int& firstData_, const int& numData_,
		const int& numBatchs_, double& lossValue_, const bool& doBack_) override;

private:
	int num_threads;

	std::vector<int> trainRangesLow, trainNums;
	std::vector<int> validRangesLow, validNums;


};