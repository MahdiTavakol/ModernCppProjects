#pragma once
#include <cmath>

class Activation
{
public:
	virtual double operator()(const double& x) = 0;
	virtual double diff(const double& x) = 0;
};

class ReLu final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
};

class Sigmoid final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
};

class FastSigmoid final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
};

class TanH final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
};
