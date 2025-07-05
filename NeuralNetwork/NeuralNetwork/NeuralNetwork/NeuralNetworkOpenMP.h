#pragma once
#include "NeuralNetwork.h"

#include <omp.h>

class NeuralNetworkOpenMP final : public NeuralNetwork
{
public:
	NeuralNetworkOpenMP(const string networkInputFileName_, const string& networkOutputFileName_,
		const int& maxNumLayers_ = 10, const int& batchsize_ = -1);
	void initializeData() override;
	void train() override;

protected:
	void backwardBatch(const MatrixXd& output_) override;

private:
	int num_threads;

	std::vector<int> trainRangesLow, trainNums;
	std::vector<int> validRangesLow, validNums;


};