#pragma once

#include <Eigen/Dense>

using Eigen::VectorXd;
using Eigen::MatrixXd;

class Optimizer
{
public:
	Optimizer();
	virtual void update();
	
protected:
	MatrixXd gradient;
};

class SGD : public Optimizer
{
public:
	SGD();
	void update() override;

private:
	double learningRate;
};

class SGDMomentum : public Optimizer
{
public:
	SGDMomentum();
	void update() override;

private:
	double gamma;
};

class RMSProp : public Optimizer
{
	RMSProp();
	void update() override;

private:
	double rho, epsilon;
};

class Adam : public Optimizer
{
	Adam();
	void update() override;

private:
	double beta1, beta2, epsilon;
};

class AdaGrad : public Optimizer
{
	AdaGrad();
	void update() override;

private:
	double epsilon;
};