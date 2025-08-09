#include "MinMaxScaler.h"
#include <iostream>

using std::cout;
using std::endl;

MatrixXd MinMaxScaler::operator()(MatrixXd& input_)
{
	double etol = 1e-6;
	VectorXd rowMin = input_.rowwise().minCoeff();
	VectorXd rowRangeInv = input_.rowwise().maxCoeff()-rowMin;
	rowRangeInv = rowRangeInv.unaryExpr([&](double x) {return x > etol ? x : 1.0; });
	MatrixXd centered = input_.colwise() - rowMin;
	MatrixXd scaled = centered.array().colwise() / rowRangeInv.array();
	
	return scaled;
}
