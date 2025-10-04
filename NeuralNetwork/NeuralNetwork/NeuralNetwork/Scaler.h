#pragma once

#include <Eigen/Dense>
#include <array>
#include <string>
#include <fstream>

using Eigen::VectorXd;
using Eigen::MatrixXd;

class Scaler
{
public:
	Scaler() = default;
	virtual MatrixXd operator()(MatrixXd& InputFile_) = 0;
	virtual ~Scaler() = default;
};

class MinMaxScaler final: public Scaler
{
public:
	MinMaxScaler() = default;
	MatrixXd operator()(MatrixXd& InputFile_) override;

private:
};

class ZScoreScaler final: public Scaler
{
public:
	ZScoreScaler() = default;
	MatrixXd operator()(MatrixXd& InputFile_) override;
private:
};
