#include "Scaler.h"
#include <iostream>

using std::cout;
using std::endl;

static constexpr double etol = 1e-6;

void MinMaxScaler::fit(MatrixXd& InputMat_)
{
	rowMin = InputMat_.rowwise().minCoeff();
	rowRange = InputMat_.rowwise().maxCoeff()-rowMin;
	rowRange = rowRange.unaryExpr([&](double x) {return x > etol ? x : 1.0; });
}

MatrixXd MinMaxScaler::transform(MatrixXd& InputMat_) const
{
	if (rowMin.size() == 0 || rowRange.size() == 0)
		throw std::invalid_argument("The transformer has not been fitted into any data yet!");
	MatrixXd centered = InputMat_.colwise() - rowMin;
	MatrixXd scaled = centered.array().colwise() / rowRange.array();
	return scaled.matrix();
}

void ZScoreScaler::fit(MatrixXd& InputMat_)
{
	rowMean = InputMat_.rowwise().mean();  // size = num_features
}

MatrixXd ZScoreScaler::transform(MatrixXd& InputMat_) const {
	if (rowMean.size() == 0)
		throw std::invalid_argument("The transformer has not been fitted into any data yet!");

    MatrixXd centered = InputMat_.colwise() - rowMean; // broadcast subtraction
    
	// variance = mean of squared deviations across columns
	VectorXd rowVar = (centered.array().square().rowwise().mean()).matrix();
	VectorXd rowStd = rowVar.array().sqrt();

	// inverse std with safe epsilon
	VectorXd rowStdInv = rowStd.unaryExpr([&](double x) {
	        return std::abs(x) > etol ? 1.0 / x : 0.0;
	});

	// scale: divide each row by its std
	MatrixXd scaled = centered.array().colwise() * rowStdInv.array();
	
	return scaled.matrix();
}
