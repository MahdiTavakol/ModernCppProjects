#include "SpringField.h"
#include "Particles.h"
#include "Error.h"

SpringField::SpringField(std::vector<Connection>& connectionInfo_) :
	ForceField{},
	connectionInfo{connectionInfo_}
{
}

SpringField::SpringField( std::vector<std::string> args_) :
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

void SpringField::init() {
	particles->getNmaxNlocal(nmax, nlocal);
	coeffMat.reserve(nmax * nmax);
	coeffMat.resize(nlocal * nlocal);
	if (coeff == -1) {
		fillCoeffMap(connectionInfo);
	}
	else
		fillCoeffMap(coeff);
}

SpringField::SpringField(double coeff_) :
	ForceField{},
	coeff{ coeff_ }
{}

void SpringField::update() {
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
				particles->X(i,0) - particles->X(j,0),
				particles->X(i,1) - particles->X(j,1),
				particles->X(i,2) - particles->X(j,2)
			};
			for (int k = 0; k < 3; k++) {
				particles->F(i, k) += -coeffMatAt(i, j) * dist[k];
				energy += 0.5 * coeffMatAt(i, j) * dist[k] * dist[k];
			}
		}
}

void SpringField::calculate_pair(std::array<double,3>& dist_,
	double* fforce_, double& energy_)
{
}

void SpringField::fillCoeffMap(const double& coeff_)
{
	for (int i = 0; i < nlocal * nlocal; i++)
		coeffMat[i] = coeff;
}

void SpringField::fillCoeffMap(const std::vector<Connection>& connectionInfo_) {
	for (auto& connect : connectionInfo_) {
		coeffMatAt(connect.atom1id, connect.atom2id) = connect.springCoeff;
		coeffMatAt(connect.atom2id, connect.atom1id) = connect.springCoeff;
	}
}