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
	virtual void fit(MatrixXd& InputMat_) = 0;
	virtual MatrixXd transform(MatrixXd& InputMat_) const = 0;
	MatrixXd operator()(MatrixXd& InputMat_) {
		return transform(InputMat_);
	};
	virtual ~Scaler() = default;
};

class MinMaxScaler final: public Scaler
{
public:
	MinMaxScaler() = default;
	void fit(MatrixXd& InputMat_) override;
	MatrixXd transform(MatrixXd& InputFile_) const override;

private:
	VectorXd rowMin, rowRange;
};

class ZScoreScaler final: public Scaler
{
public:
	ZScoreScaler() = default;
	void fit(MatrixXd& InputMat_) override;
	MatrixXd transform(MatrixXd& InputFile_) const override;
private:
	VectorXd rowMean;
};
