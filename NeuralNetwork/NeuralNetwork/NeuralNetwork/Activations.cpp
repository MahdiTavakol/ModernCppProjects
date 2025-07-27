#include "Activations.h"

double ReLu::operator()(const double& x)
{
	return (x + std::abs(x)) / 2.0;
}

double ReLu::diff(const double& x)
{
	return (1.0 + x / std::abs(x)) / 2.0;
}

MatrixXd ReLu::operator()(const MatrixXd& mat_)
{
	return mat_.unaryExpr([&](double x) {return (x + std::abs(x)) / 2.0; });
}

MatrixXd ReLu::diff(const MatrixXd& mat_)
{
	return mat_.unaryExpr([&](double x) {return (1.0 + x / std::abs(x)) / 2.0; });
}

double LeakyRelu::operator()(const double& x)
{
	if (x >= 0) return x;
	else return alpha * x;
}

double LeakyRelu::diff(const double& x)
{
	if (x > 0) return 1.0;
	else if (x < 0) return -alpha;
	else return (1.0 + alpha) / 2.0;
}

MatrixXd LeakyRelu::operator()(const MatrixXd& mat_)
{
	auto lambda = [&](double x)
		{
			if (x >= 0) return x;
			else return alpha * x;
		};
	return mat_.unaryExpr(lambda);
}

MatrixXd LeakyRelu::diff(const MatrixXd& mat_)
{
	auto lambda = [&](double x)
		{
			if (x > 0) return 1.0;
			else if (x < 0) return -alpha;
			else return (1.0 + alpha) / 2.0;
		};
	return mat_.unaryExpr(lambda);
}

double Selu::operator()(const double& x)
{
	if (x > 0) return lambda * x;
	else if (x < 0) return lambda * alpha * (std::exp(x) - 1);
}

double Selu::diff(const double& x)
{
	if (x > 0) return lambda;
	else if (x < 0) return lambda * alpha * std::exp(x);
	else return lambda * (alpha + 1.0) / 2.0;
}

double Sigmoid::operator()(const double& x)
{
	return (1.0 / (1.0 + std::exp(-x)));
}

double Sigmoid::diff(const double& x)
{
	return std::exp(-x) / ((1.0 + std::exp(-x)) * (1.0 + std::exp(-x)));
}

double FastSigmoid::operator()(const double& x)
{
	return x / (1 + std::abs(x));
}

double FastSigmoid::diff(const double& x)
{
	return ((1 + std::abs(x)) + x * (x/std::abs(x))) / ((1 + std::abs(x))* (1 + std::abs(x)));
}

double TanH::operator()(const double& x)
{
	return std::tanh(x);
}

double TanH::diff(const double& x)
{
	return 1.0 / (std::cosh(x)* std::cosh(x));
}
