#include "Dropout.h"

Dropout::Dropout(const double  _rate):
	rate{_rate}, gen{rd()}, dist{0.0,1.0}
{}

void Dropout::initialize()
{
	auto vectorDrop = [&](const double /*no used*/)->double {
		double randomNumber = dist(gen);
		return (randomNumber < rate ? 0.0 : 1.0/(1-rate));
	};
	for (int j = 0; j < weights.cols(); j++)
		weights.col(j) = weights.col(j).unaryExpr(vectorDrop);
	);
	weights.col(weights.cols()-1).setZero();
}

MatrixXd Dropout::forward(const MatrixXd& input_)
{
	if (weights.cols() != input_.cols() || weights.rows() != input_.rows())
	{
		weights.resize(input_.rows(),input_.cols());
	}
	this->initialize();
	return LayerBatchEfficient::forward(input_);
}

MatrixXd Dropout::backward(MatrixXd& nextDiff_)
{
	return weights.block(0, 0, weights.rows(), weights.cols() - 1).transpose() * nextDiff_;
}

MatrixXd Dropout::update(const MatrixXd& input_)
{
 	// do nothing.
}
	


