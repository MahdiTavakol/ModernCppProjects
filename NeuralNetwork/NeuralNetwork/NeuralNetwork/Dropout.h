#pragma once
#include <Eigen/Dense>
#include <random>
#include "LayerBatchEfficient.h"
#include "Logger.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;

class Dropout : public LayerBatchEfficient
{
public:
	Dropout(Logger& logger_, const int& batchsize_, const int& Dim_,const double& dropRate_);
	Dropout(Logger& logger_, const int& Dim_, const double& dropRate_);
	void initialize() override;
	MatrixXd forward(const MatrixXd& InputFile_, const bool trainMode = true) override;
	MatrixXd backward(MatrixXd& nextDiff_) override;
	void update(const std::unique_ptr<Optimizer>& OptFuncPtr_) override;
private:
	double dropRate;
	double scaleRate;
	
	
	std::random_device rd;
	std::mt19937_64 gen;
	std::unique_ptr<std::bernoulli_distribution> bern_keep;
	VectorXd mask_vec;
	MatrixXd mask_full;


	void resetMask();
};

