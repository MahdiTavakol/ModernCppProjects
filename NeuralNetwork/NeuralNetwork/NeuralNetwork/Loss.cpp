#include "Loss.h"
#include <numeric>

double MSE::operator()(const VectorXd& output_, const VectorXd& real_)
{
	VectorXd difference = output_ - real_;
	return difference.array().square().sum() / output_.size();
}

VectorXd MSE::diff(const VectorXd& output_, const VectorXd& real_)
{
	VectorXd difference = (2.0 / output_.size()) * (output_ - real_);
	return difference;
}

double MAE::operator()(const VectorXd& output_, const VectorXd& real_)
{
	VectorXd difference = output_ - real_;
	difference.array().abs().sum() / output_.size();
}

VectorXd MAE::diff(const VectorXd& output_, const VectorXd& real_)
{
	VectorXd difference = (output_ - real_).array().sign() / output_.size();
	return difference;
}