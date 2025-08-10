#pragma once
#include <cmath>
#include <vector>
#include <Eigen/Dense>

using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::RowVectorXd;
using std::vector;

class Activation
{
public:
	virtual double operator()(const double& x) = 0;
	virtual double diff(const double& x) = 0;
	virtual MatrixXd operator()(const MatrixXd& mat_) = 0;
	virtual MatrixXd diff(const MatrixXd& mat_) = 0;
};

class None final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
	MatrixXd operator()(const MatrixXd& mat_) override;
	virtual MatrixXd diff(const MatrixXd& mat_) override;
};

class ReLu final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
	MatrixXd operator()(const MatrixXd& mat_) override;
	virtual MatrixXd diff(const MatrixXd& mat_) override;
};

class LeakyRelu final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
	MatrixXd operator()(const MatrixXd& mat_) override;
	virtual MatrixXd diff(const MatrixXd& mat_) override;
private:
	double alpha = 0.01;
};

class Selu final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
	MatrixXd operator()(const MatrixXd& mat_) override;
	virtual MatrixXd diff(const MatrixXd& mat_) override;
private:
	double alpha = 1.0507, lambda = 1.6733;
};

class Sigmoid final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
	MatrixXd operator()(const MatrixXd& mat_) override;
	virtual MatrixXd diff(const MatrixXd& mat_) override;
};

class FastSigmoid final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
	MatrixXd operator()(const MatrixXd& mat_) override;
	virtual MatrixXd diff(const MatrixXd& mat_) override;
};

class TanH final : public Activation
{
public:
	double operator()(const double& x) override;
	double diff(const double& x) override;
	MatrixXd operator()(const MatrixXd& mat_) override;
	virtual MatrixXd diff(const MatrixXd& mat_) override;
};

class SoftMaxLastLayer final : public Activation
{
public:
	MatrixXd operator()(const MatrixXd& mat_) override;
	virtual MatrixXd diff(const MatrixXd& mat_) override;
	vector<MatrixXd> diff_vec(const MatrixXd& mat_);
};
