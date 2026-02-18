#pragma once

#include "ForceField.h"

class SpringField : public ForceField {
public:
	SpringField(Engine& engine_, double coeff_) :
		ForceField{ engine_ },
		coeff{coeff_}
	{}

protected:
	void calculate_pair(std::array<double,3>& dist,
		double* fforce_, double& energy_) override;
	double coeff;
};

class SpringFormula {
public:
	SpringFormula(double& coeff_) :
		coeff{ coeff_ } 
	{}
	virtual void operator()(const double& dist_, double& force_, double& energy_) = 0;
protected:
	double coeff;
};

class SpringFormula1 : public SpringFormula {
public:
	SpringFormula1(double& coeff_) :
		SpringFormula{coeff_}
	{}
	void operator()(const double& dist_, double& force_, double& energy_) {
		force_ = coeff * dist_;
		energy_ = 0.5 * coeff * dist_ * dist_;
	}
};