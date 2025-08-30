#pragma once
#include <Eigen/Dense>
#include "LayerBatchEfficient.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;

class Dropout: public LayerBatchEfficient
{
public:
	Dropout(const _rate=0.1);
	void initialize() override;
	MatrixXd forward(const MatrixXd& input_) override;
	MatrixXd backward(MatrixXd& nextDiff_) override;
	void update();
private:
	double rate;
	std::random_device rd;
	std::19937_64 gen;
	std::uniform_real_distribution dist;
}

