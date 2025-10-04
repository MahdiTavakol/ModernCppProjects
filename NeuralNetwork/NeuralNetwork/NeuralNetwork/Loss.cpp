#include "Loss.h"
#include <numeric>

double MSE::operator()(const MatrixXd& output_, const MatrixXd& real_)
{
	MatrixXd difference = output_ - real_;
	return difference.array().square().mean();
}

MatrixXd MSE::diff(const MatrixXd& output_, const MatrixXd& real_)
{
	MatrixXd difference = 2.0 * (output_ - real_)/output_.cols();
	return difference;
}

double MAE::operator()(const MatrixXd& output_, const MatrixXd& real_)
{
	MatrixXd difference = output_ - real_;
	return difference.array().abs().mean();
}

MatrixXd MAE::diff(const MatrixXd& output_, const MatrixXd& real_)
{
	MatrixXd difference = (output_ - real_).unaryExpr([&](double v) {return v / std::abs(v); })/output_.cols();
	return difference;
}

double Huber::operator()(const MatrixXd& output_, const MatrixXd& real_)
{
	MatrixXd difference = (output_ - real_).unaryExpr([&](double v) {return lFunction(v); });
	return difference.array().sum();
}

MatrixXd Huber::diff(const MatrixXd& output_, const MatrixXd& real_)
{
	MatrixXd difference = (output_ - real_).unaryExpr([&](double v) {return lFunctionDiff(v); });
	return difference;
}

double Huber::lFunction(const double& InputFile_)
{
	if (std::abs(InputFile_) <= delta)
		return 0.5 * InputFile_ * InputFile_;
	return delta * (std::abs(InputFile_) - 0.5 * delta);
}

double Huber::lFunctionDiff(const double& InputFile_)
{
	if (std::abs(InputFile_) <= delta)
		return InputFile_;
	return delta * InputFile_ / std::abs(InputFile_);
}

