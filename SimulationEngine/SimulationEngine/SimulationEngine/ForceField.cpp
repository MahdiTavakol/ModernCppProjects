#include "ForceField.h"
#include "Particles.h"
#include <iostream>

ForceField::ForceField(Engine& engine_) :
	Ref{ engine_,"1" }
{}


void ForceField::init() {
	int nlocal, nmax;
	return;
	auto& particles = engine().getParticlesForUpdate();
	auto f = particles->getFData();
	particles->getNmaxNlocal(nmax, nlocal);

	for (int i = 0; i < nlocal; i++) {
		f[3 * i] = 0.0;
		f[3 * i + 1] = 0.0;
		f[3 * i + 2] = 0.0;
	}
}

void ForceField::update() {
	int nlocal, nmax;
	auto& box = engine().getBox();
	auto& particles = engine().getParticlesForUpdate();
	auto x = particles->getXData();
	auto f = particles->getFData();
	particles->getNmaxNlocal(nmax, nlocal);

	double energy;
	for (int i = 0; i < nlocal; i++)
		for (int j = 0; j < nlocal; j++) {
			if (j == i) continue;
			std::array<double, 3> dist = {
				x[3 * i] - x[3 * j],
				x[3 * i + 1] - x[3 * j + 1],
				x[3 * i + 2] - x[3 * j + 2],
			};
			calculate_pair(dist, &f[3 * i], energy);
		}
}