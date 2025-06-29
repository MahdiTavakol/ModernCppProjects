#include "Activations.h"

double ReLu::operator()(const double& x)
{
	return (x + std::abs(x)) / 2.0;
}

double ReLu::diff(const double& x)
{
	return (1.0 + x / std::abs(x)) / 2.0;
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
