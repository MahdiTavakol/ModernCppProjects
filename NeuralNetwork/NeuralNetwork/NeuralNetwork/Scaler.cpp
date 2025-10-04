#include "Scaler.h"
#include <iostream>

using std::cout;
using std::endl;

MatrixXd MinMaxScaler::operator()(MatrixXd& InputFile_)
{
	double etol = 1e-6;
	VectorXd rowMin = InputFile_.rowwise().minCoeff();
	VectorXd rowRange = InputFile_.rowwise().maxCoeff()-rowMin;
	rowRange = rowRange.unaryExpr([&](double x) {return x > etol ? x : 1.0; });
	MatrixXd centered = InputFile_.colwise() - rowMin;
	MatrixXd scaled = centered.array().colwise() / rowRange.array();
	
	return scaled;
}

MatrixXd ZScoreScaler::operator()(MatrixXd& InputFile_)
{
	double etol = 1e-6;
	VectorXd rowMean = InputFile_.rowwise().mean();  // size = num_features
    	MatrixXd centered = InputFile_.colwise() - rowMean; // broadcast subtraction
    
	// variance = mean of squared deviations across columns
	VectorXd rowVar = (centered.array().square().rowwise().mean()).matrix();
	VectorXd rowStd = rowVar.array().sqrt();

	// inverse std with safe epsilon
	VectorXd rowStdInv = rowStd.unaryExpr([&](double x) {
	        return std::abs(x) > etol ? 1.0 / x : 1.0;
	});

	// scale: divide each row by its std
	MatrixXd scaled = centered.array().colwise() * rowStdInv.array();
	
	return scaled;
}
