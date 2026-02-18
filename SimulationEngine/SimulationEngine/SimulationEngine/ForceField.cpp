#include "ForceField.h"
#include "Particles.h"

void ForceField::update() {
	int nlocal, nmax;
	auto& particles = engine().getParticlesForUpdate();
	auto x = particles->getXData();
	auto f = particles->getFData();
	particles->getNmaxNlocal(nmax, nlocal);

	double energy;
	for (int i = 0; i < nmax; i++)
		for (int j = 0; j < nmax; j++) {
			if (j == i) continue;
			std::array<double, 3> dist = {
				x[3 * i] - x[3 * j],
				x[3 * i + 1] - x[3 * j + 1],
				x[3 * i + 2] - x[3 * j + 2],
			};
			calculate_pair(dist, &f[3 * i], energy);
		}
}