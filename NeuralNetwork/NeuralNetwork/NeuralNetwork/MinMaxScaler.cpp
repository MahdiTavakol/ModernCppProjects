#include "MinMaxScaler.h"

MatrixXd MinMaxScaler::operator()(MatrixXd& input_)
{
	double etol = 1e-6;
	VectorXd rowMin = input_.rowwise().minCoeff();
	VectorXd rowRange = input_.rowwise().maxCoeff()-rowMin;
	rowRange = rowRange.unaryExpr([&](double x) {return x > etol ? x : 0.0; });
	MatrixXd centered = input_.colwise() - rowMin;
	MatrixXd scaled = centered.array().colwise() / rowRange.array();
	return scaled;
}