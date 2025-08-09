#include "Activations.h"
#include <iostream>

double ReLu::operator()(const double& x)
{
	return (x + std::abs(x)) / 2.0;
}

double ReLu::diff(const double& x)
{
	double etol = 1e-6;
	if (std::abs(x) > etol)
		return (1.0 + x / std::abs(x)) / 2.0;
	if (x<0)
		return -1.0;
	return 1.0;
}

MatrixXd ReLu::operator()(const MatrixXd& mat_)
{
	return mat_.unaryExpr([&](double x) {return (x + std::abs(x)) / 2.0; });
}

MatrixXd ReLu::diff(const MatrixXd& mat_)
{
	double etol = 1e-6;
	return mat_.unaryExpr([&](double x) {
		if (std::abs(x) > etol)
			return (1.0 + x / std::abs(x)) / 2.0;
		if (x<0) return -1.0;
		return 1.0; 
	});
}

double LeakyRelu::operator()(const double& x)
{
	if (x >= 0) return x;
	else return alpha * x;
}

double LeakyRelu::diff(const double& x)
{
	if (x > 0) return 1.0;
	else if (x < 0) return -alpha;
	else return (1.0 + alpha) / 2.0;
}

MatrixXd LeakyRelu::operator()(const MatrixXd& mat_)
{
	auto lambda = [&](double x)
		{
			if (x >= 0) return x;
			else return alpha * x;
		};
	return mat_.unaryExpr(lambda);
}

MatrixXd LeakyRelu::diff(const MatrixXd& mat_)
{
	auto lambda = [&](double x)
		{
			if (x > 0) return 1.0;
			else if (x < 0) return -alpha;
			else return (1.0 + alpha) / 2.0;
		};
	return mat_.unaryExpr(lambda);
}

double Selu::operator()(const double& x)
{
	if (x > 0) return lambda * x;
	else if (x < 0) return lambda * alpha * (std::exp(x) - 1);
}

double Selu::diff(const double& x)
{
	if (x > 0) return lambda;
	else if (x < 0) return lambda * alpha * std::exp(x);
	else return lambda * (alpha + 1.0) / 2.0;
}

MatrixXd Selu::operator()(const MatrixXd& mat_)
{
	auto func = [&](double x)
		{
			if (x > 0) return lambda * x;
			else if (x < 0) return lambda * alpha * (std::exp(x) - 1);
		};
	return mat_.unaryExpr(func);
}

MatrixXd Selu::diff(const MatrixXd& mat_)
{
	auto func = [&](double x)
		{
			if (x > 0) return lambda;
			else if (x < 0) return lambda * alpha * std::exp(x);
			else return lambda * (alpha + 1.0) / 2.0;
		};
	return mat_.unaryExpr(func);
}

double Sigmoid::operator()(const double& x)
{
	return (1.0 / (1.0 + std::exp(-x)));
}

double Sigmoid::diff(const double& x)
{
	return std::exp(-x) / ((1.0 + std::exp(-x)) * (1.0 + std::exp(-x)));
}

MatrixXd Sigmoid::operator()(const MatrixXd& mat_)
{
	auto func = [&](double x)
		{
			return (1.0 / (1.0 + std::exp(-x)));
		};
	return mat_.unaryExpr(func);
}

MatrixXd Sigmoid::diff(const MatrixXd& mat_)
{
	auto func = [&](double x)
		{
			return std::exp(-x) / ((1.0 + std::exp(-x)) * (1.0 + std::exp(-x)));
		};
	return mat_.unaryExpr(func);
}

double FastSigmoid::operator()(const double& x)
{
	return x / (1 + std::abs(x));
}

double FastSigmoid::diff(const double& x)
{
	return ((1 + std::abs(x)) + x * (x/std::abs(x))) / ((1 + std::abs(x))* (1 + std::abs(x)));
}

MatrixXd FastSigmoid::operator()(const MatrixXd& mat_)
{
	return mat_.unaryExpr([&](double x) {return x / (1 + std::abs(x)); });
}

MatrixXd FastSigmoid::diff(const MatrixXd& mat_)
{
	return mat_.unaryExpr([&](double x) {
		return ((1 + std::abs(x)) + x * (x / std::abs(x))) / ((1 + std::abs(x)) * (1 + std::abs(x)));
		});
}

double TanH::operator()(const double& x)
{
	return std::tanh(x);
}

double TanH::diff(const double& x)
{
	return 1.0 / (std::cosh(x)* std::cosh(x));
}

MatrixXd TanH::operator()(const MatrixXd& mat_)
{
	return mat_.unaryExpr([&](double x) {return std::tanh(x); });
}

MatrixXd TanH::diff(const MatrixXd& mat_)
{
	return mat_.unaryExpr([&](double x) {return 1.0 / (std::cosh(x) * std::cosh(x)); });
}

MatrixXd SoftMaxLastLayer::operator()(const MatrixXd& mat_)
{
	RowVectorXd colMax = mat_.colwise().maxCoeff();
	MatrixXd mat = mat_ - colMax.replicate(mat_.rows(), 1);
	MatrixXd expMat = mat.unaryExpr([&](double elem) {return exp(elem); });
	RowVectorXd coeffRow = expMat.colwise().sum();

	for (int j = 0; j < mat_.cols(); j++)
	{
		expMat.col(j) = expMat.col(j).array()/coeffRow[j];
	}

	return expMat;
}

MatrixXd SoftMaxLastLayer::diff(const MatrixXd& mat_)
{
	/**/
	std::cout << "Warning: this function returns a Zero matrix" 
		<< "You should use the diff_vec function instead!" << std::endl;
	return MatrixXd::Zero(mat_.rows(), mat_.cols());
}

vector<MatrixXd> SoftMaxLastLayer::diff_vec(const MatrixXd& mat_)
{
	vector<MatrixXd> vec;
	vec.reserve(mat_.cols());
	MatrixXd val = this->operator()(mat_);
	for (int k = 0; k < mat_.cols(); k++)
	{
		MatrixXd mat_k{ mat_.rows(),mat_.rows() };
		for (int i = 0; i < mat_.rows(); i++)
		{
			double vi = val(i, k);
			for (int j = 0; j < mat_.rows(); j++)
				if (i == j)
					mat_k(i, j) = vi * (1 - val(i, k));
				else
					mat_k(i, j) = -vi * val(j, k);
		}
		vec.push_back(mat_k);
	}
	return vec;
}


