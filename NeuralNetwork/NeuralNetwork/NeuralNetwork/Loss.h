#pragma once
#include <Eigen/Dense>

using Eigen::VectorXd;
using Eigen::MatrixXd;

class Loss
{
public:
	virtual double operator()(const MatrixXd& output_, const MatrixXd& real_) = 0;
	virtual MatrixXd diff(const MatrixXd& output_, const MatrixXd& real_) = 0;
};

class MSE final : public Loss
{
public:
	double operator()(const MatrixXd& output_, const MatrixXd& real_) override;
	MatrixXd diff(const MatrixXd& output_, const MatrixXd& real_) override;
};

class MAE final : public Loss
{
public:
	double operator()(const MatrixXd& output_, const MatrixXd& real_) override;
	MatrixXd diff(const MatrixXd& output_, const MatrixXd& real_) override;
};

class Huber final : public Loss
{
public:
	Huber(const double& delta_) :delta{ delta_ } {};
	Huber() : delta{ 1.0 } {}
	double operator()(const MatrixXd& output_, const MatrixXd& real_) override;
	MatrixXd diff(const MatrixXd& output_, const MatrixXd& real_) override;
private:
	double delta;
	double lFunction(const double& input_);
	double lFunctionDiff(const double& input_);
};