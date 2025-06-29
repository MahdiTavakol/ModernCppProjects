#pragma once
#include <cmath>

class Activation
{
public:
	virtual double operator()(const double& x) = 0;
};

class ReLu : public Activation
{
public:
	double operator()(const double& x)
	{
		return (x + std::abs(x)) / 2.0;
	}
};

class Sigmoid : public Activation
{
public:
	double operator()(const double& x)
	{
		return (1.0 / (1.0 + std::exp(-x)));
	}
};

class FastSigmoid : public Activation
{
public:
	double operator()(const double& x)
	{
		return x / (1 + std::abs(x));
	}
};

class TanH : public Activation
{
public:
	double operator()(const double& x)
	{
		return std::tanh(x);
	}
};
