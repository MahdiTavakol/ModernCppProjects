#pragma once
#include <Eigen/Dense>

using Eigen::VectorXd;

class Loss
{
public:
	virtual double operator()(const VectorXd& output_, const VectorXd& real_) = 0;
	virtual VectorXd diff(const VectorXd& output_, const VectorXd& real_) = 0;
};

class MSE final : public Loss
{
public:
	double operator()(const VectorXd& output_, const VectorXd& real_) override;
	VectorXd diff(const VectorXd& output_, const VectorXd& real_) override;
};

class MAE final : public Loss
{
public:
	double operator()(const VectorXd& output_, const VectorXd& real_) override;
	VectorXd diff(const VectorXd& output_, const VectorXd& real_) override;
};

class Huber final : public Loss
{
public:
	double operator()(const VectorXd& output_, const VectorXd& real_) override;
	VectorXd diff(const VectorXd& output_, const VectorXd& real_) override;
};