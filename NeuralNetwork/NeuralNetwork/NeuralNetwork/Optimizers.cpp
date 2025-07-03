#include "Optimizers.h"

Optimizer::Optimizer(const int& learningRate_):
	learningRate{learningRate_}
{}

SGD::SGD(const int& learningRate_):
	Optimizer{learningRate_}
{}

void SGD::update(MatrixXd& weights_, const MatrixXd& gradient_)
{
	weights_ -= learningRate * gradient_;
}

SGDMomentum::SGDMomentum(const int& learningRate_, const double& gamma_):
	Optimizer{learningRate_}, gamma{gamma_}
{}

SGDMomentum::SGDMomentum(const int& learningRate_):
	SGDMomentum{learningRate_,0.9}
{}

void SGDMomentum::update(MatrixXd& weights_, const MatrixXd& gradient_)
{
	weightsVelocity = gamma * weightsVelocity + learningRate * gradient_;
	weights_ -= weightsVelocity;
}

RMSProp::RMSProp(const int& learningRate_, const double& rho_):
	Optimizer{learningRate_}, rho{rho_}, epsilon{1e-8}
{}

RMSProp::RMSProp(const int& learningRate_) :
	RMSProp{learningRate_ ,0.7}
{}

void RMSProp::update(MatrixXd& weights_, const MatrixXd& gradient_)
{
	MatrixXd gradient2 = gradient_.unaryExpr([&](double v) {return v * v; });
	st = rho * st + (1 - rho) * gradient2;
	MatrixXd radicalSt = st.unaryExpr([&](double v) {return learningRate / std::sqrt(v + epsilon); });
	weights_  -= gradient_.cwiseProduct(radicalSt);
}

Adam::Adam(const int& learningRate_, const double& beta1_, const double& beta2_):
	Optimizer{learningRate_}, beta1{beta1_}, beta2{beta2_}, epsilon{1e-8}
{}

Adam::Adam(const int& learningRate_) :
	Adam{learningRate_,0.9,0.999}
{}

void Adam::update(MatrixXd& weights_, const MatrixXd& gradient_)
{
	firstMomentum = beta1 * firstMomentum + (1 - beta1) * gradient_;
	MatrixXd gradient2 = gradient_.unaryExpr([&](double v) {return v * v; });
	secondMomentum = beta2 * secondMomentum + (1.0 - beta2) * gradient2;
	firstMomentum = firstMomentum / (1.0 - beta1);
	secondMomentum = secondMomentum / (1.0 - beta2);
	MatrixXd secondMomentumInv = secondMomentum.unaryExpr([&](double v) {return 1.0 / (sqrt(v) + epsilon); });
	weights_ -= learningRate * firstMomentum.cwiseProduct(secondMomentumInv);
}

AdaGrad::AdaGrad(const int& learningRate_):
	Optimizer{learningRate_}, epsilon{1e-8}
{}

void AdaGrad::update(MatrixXd& weights_, const MatrixXd& gradient_)
{
	squareGradient += gradient_.unaryExpr([&](double v) {return v * v; });
	MatrixXd invGradient = gradient_.unaryExpr([&](double v) {return 1.0 / (std::sqrt(v) + epsilon); });
	weights_ -= learningRate * invGradient.cwiseProduct(gradient_);
}
