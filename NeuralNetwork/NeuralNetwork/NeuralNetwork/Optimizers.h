#pragma once

#include <Eigen/Dense>

using Eigen::VectorXd;
using Eigen::MatrixXd;

class Optimizer
{
public:
	Optimizer(const double& learningRate_);
	virtual void update(MatrixXd& weights_, const MatrixXd& gradient_) = 0;
	
protected:
	double learningRate;
};

class SGD : public Optimizer
{
public:
	SGD(const double& learningRate_);
	void update(MatrixXd& weights_, const MatrixXd& gradient_) override;

};

class SGDMomentum : public Optimizer
{
public:
	SGDMomentum(const double& learningRate_, const double& gamma_);
	SGDMomentum(const double& learningRate_);
	void update(MatrixXd& weights_, const MatrixXd& gradient_) override;

private:
	double gamma;
	MatrixXd weightsVelocity;
};

class RMSProp : public Optimizer
{
public:
	RMSProp(const double& learningRate_, const double& rho_);
	RMSProp(const double& learningRate_);
	void update(MatrixXd& weights_, const MatrixXd& gradient_) override;

private:
	double rho, epsilon;
	MatrixXd st;
};

class Adam : public Optimizer
{
public:
	Adam(const double& learningRate_, const double& beta1, const double& beta2);
	Adam(const double& learningRate_);
	void update(MatrixXd& weights_, const MatrixXd& gradient_) override;

private:
	double beta1, beta2, epsilon;
	MatrixXd firstMomentum, secondMomentum;
};

class AdaGrad : public Optimizer
{
public:
	AdaGrad(const double& learningRate_);
	void update(MatrixXd& weights_, const MatrixXd& gradient_) override;

private:
	double epsilon;
	MatrixXd squareGradient;
};