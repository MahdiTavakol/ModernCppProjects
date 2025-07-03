#pragma once
#include "NeuralNetwork.h"

class NeuralNetworkMPI final : public NeuralNetwork
{
public:
	NeuralNetworkMPI(const string networkInputFileName_, const string& networkOutputFileName_,
		const int& maxNumLayers_ = 10, const int& batchsize_ = -1);

protected:
	void trainBatches(const double& firstData_, const double& numData_, const int& numBatchs_, double& lossValue_, const bool& doBack_) override;
};