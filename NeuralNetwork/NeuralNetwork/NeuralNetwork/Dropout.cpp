#include "Dropout.h"

Dropout::Dropout(Logger& logger_, const int& batchsize_, const int& Dim_, const double& learningRate_, const double& dropRate_):
	LayerBatchEfficient{logger_,batchsize_,Dim_,Dim_,ActivationType::NONE,OptimizerType::NONE,learningRate_}, dropRate{dropRate_}
{
	if (dropRate < 0.0 || dropRate >= 1.0)
		throw std::invalid_argument("The rate value must be in the (0,1] range!");
	
}

Dropout::Dropout(Logger& logger_, const int& Dim_, const double& learningRate_, const double& dropRate_) :
	Dropout(logger_, 32, Dim_, learningRate_, dropRate_) {}


void Dropout::initialize()
{
	gen.seed(rd());
	bern_keep = std::make_unique<std::bernoulli_distribution>(1.0 - dropRate);
	scaleRate = 1.0 / (1.0 - dropRate);
	mask_vec.resize(InputFileDim);
	mask_full.resize(0, 0);
}

MatrixXd Dropout::forward(const MatrixXd& InputFile_, const bool trainMode)
{
	if (!trainMode)
		return InputFile_;


	// A new sets of weights
	if (trainMode) {
		this->resetMask();
		mask_full = mask_vec.rowwise().replicate(InputFile_.cols());
		return InputFile_.cwiseProduct(mask_full);
	} else {
		// do not change in the transforming mode
		return InputFile_;
	}
}

MatrixXd Dropout::backward(MatrixXd& nextDiff_)
{
	if (mask_full.size() == 0) return nextDiff_;
	return nextDiff_.cwiseProduct(mask_full);
}

void Dropout::update()
{
 	// do nothing.
}

void Dropout::resetMask() {
	if (mask_vec.size() != InputFileDim) mask_vec.resize(InputFileDim);

	for (int i = 0; i < InputFileDim; i++) {
		mask_vec[i] = (*bern_keep)(gen) ? scaleRate : 0.0;
	}
}
	


