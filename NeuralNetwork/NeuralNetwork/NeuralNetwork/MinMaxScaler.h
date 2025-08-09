#pragma once

#include <Eigen/Dense>
#include <array>
#include <string>
#include <fstream>

using Eigen::VectorXd;
using Eigen::MatrixXd;

class MinMaxScaler
{
public:
	MinMaxScaler() = default;
	MatrixXd operator()(MatrixXd& input_);

private:
};
