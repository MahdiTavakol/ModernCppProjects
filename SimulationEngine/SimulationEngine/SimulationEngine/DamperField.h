#pragma once

#include "ForceField.h"

class DamperField : public ForceField {
public:
	DamperField(std::vector<Connection>& connectionInfo_);
	DamperField(std::vector<std::string> args_);
	DamperField(double coeff_);
	void init() override;
	void update() override;

protected:
	// the number of particles 
	int nmax, nlocal;
	void calculate_pair(std::array<double, 3>& dist,
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
