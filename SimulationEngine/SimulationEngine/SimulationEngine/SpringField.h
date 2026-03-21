#pragma once

#include "ForceField.h"


class SpringField : public ForceField {
public:
	SpringField(std::vector<Connection>& connectionInfo_);
	SpringField(std::vector<std::string> args_);
	SpringField(double coeff_);
	void init() override;
	void update() override;

protected:
	// the number of particles 
	int nmax, nlocal;
	void calculate_pair(std::array<double,3>& dist,
		double* fforce_, double& energy_) override;
	double coeff = -1;
	std::vector<double> coeffMat;
	std::vector<Connection> connectionInfo;

	void fillCoeffMap(const double& coeff);
	void fillCoeffMap(const std::vector<Connection>& connectionInfo_);
	inline double& coeffMatAt(const int& i, const int& j) {
		return coeffMat[i * nlocal + j];
	}
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