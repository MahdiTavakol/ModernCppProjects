#pragma once

#include <Eigen/Dense>

using Eigen::VectorXd;
using Eigen::MatrixXd;

class Optimizer
{
public:
	Optimizer(const int& learningRate_);
	virtual void update(MatrixXd& weights_, const MatrixXd& gradient_) = 0;
	
protected:
	double learningRate;
};

class SGD : public Optimizer
{
public:
	SGD(const int& learningRate_);
	void update(MatrixXd& weights_, const MatrixXd& gradient_) override;

};

class SGDMomentum : public Optimizer
{
public:
	SGDMomentum(const int& learningRate_, const double& gamma_);
	SGDMomentum(const int& learningRate_);
	void update(MatrixXd& weights_, const MatrixXd& gradient_) override;

private:
	double gamma;
	MatrixXd weightsVelocity;
};

class RMSProp : public Optimizer
{
	RMSProp(const int& learningRate_, const double& rho_);
	RMSProp(const int& learningRate_);
	void update(MatrixXd& weights_, const MatrixXd& gradient_) override;

private:
	double rho, epsilon;
	MatrixXd st;
};

class Adam : public Optimizer
{
	Adam(const int& learningRate_, const double& beta1, const double& beta2);
	Adam(const int& learningRate_);
	void update(MatrixXd& weights_, const MatrixXd& gradient_) override;

private:
	double beta1, beta2, epsilon;
	MatrixXd firstMomentum, secondMomentum;
};

class AdaGrad : public Optimizer
{
	AdaGrad(const int& learningRate_);
	void update(MatrixXd& weights_, const MatrixXd& gradient_) override;

private:
	double epsilon;
	MatrixXd squareGradient;
};