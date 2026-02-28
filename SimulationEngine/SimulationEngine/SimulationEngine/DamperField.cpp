#include "DamperField.h"
#include "Particles.h"
#include "Error.h"

DamperField::DamperField(std::vector<Connection>& connectionInfo_) :
	ForceField{},
	connectionInfo{ connectionInfo_ }
{
}

DamperField::DamperField(std::vector<std::string> args_) :
	ForceField{ args_ }
{
	auto nargs = args_.size();
	if (nargs < 3) {
		//error->one("Not enough arguments for springfield command! Expected at least 3, got " + std::to_string(nargs));
		coeff = 0.0;
		return;
	}
	coeff = std::stod(args_[2]);

}

void DamperField::init() {
	particles->getNmaxNlocal(nmax, nlocal);
	coeffMat.reserve(nmax * nmax);
	coeffMat.resize(nlocal * nlocal);
	if (coeff == -1) {
		fillCoeffMap(connectionInfo);
	}
	else
		fillCoeffMap(coeff);
}

DamperField::DamperField(double coeff_) :
	ForceField{},
	coeff{ coeff_ }
{
}

void DamperField::update() {
	resetForce();
	int nlocal_new, nmax_new;
	particles->getNmaxNlocal(nmax_new, nlocal_new);
	if (nlocal_new != nlocal) {
		coeffMat.resize(nlocal * nlocal);
		nlocal = nlocal_new;
		if (coeff == -1)
			fillCoeffMap(connectionInfo);
		else
			fillCoeffMap(coeff);
		nlocal = nlocal_new;
	}
	if (nmax_new != nmax) {
		nmax = nmax_new;
		coeffMat.reserve(nmax * nmax);
	}



	double energy = 0.0;
	for (int i = 0; i < nlocal; i++)
		for (int j = 0; j < nlocal; j++) {
			if (j == i) continue;
			std::array<double, 3> dist = {
				particles->V(i,0) - particles->V(j,0),
				particles->V(i,1) - particles->V(j,1),
				particles->V(i,2) - particles->V(j,2)
			};
			for (int k = 0; k < 3; k++) {
				particles->F(i, k) += -coeffMatAt(i, j) * dist[k];
				energy += 0.5 * coeffMatAt(i, j) * dist[k] * dist[k];
			}
		}
}

void DamperField::calculate_pair(std::array<double, 3>& dist_,
	double* fforce_, double& energy_)
{
}

void DamperField::fillCoeffMap(const double& coeff_)
{
	for (int i = 0; i < nlocal * nlocal; i++)
		coeffMat[i] = coeff;
}

void DamperField::fillCoeffMap(const std::vector<Connection>& connectionInfo_) {
	for (auto& connect : connectionInfo_) {
		coeffMatAt(connect.atom1id, connect.atom2id) = connect.springCoeff;
		coeffMatAt(connect.atom2id, connect.atom1id) = connect.springCoeff;
	}
}