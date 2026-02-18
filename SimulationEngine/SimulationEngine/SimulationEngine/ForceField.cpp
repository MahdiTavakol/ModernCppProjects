#include "ForceField.h"
#include "Particles.h"
#include <iostream>

void ForceField::update() {
	int nlocal, nmax;
	std::cout << "1" << std::endl;
	auto& particles = engine().getParticlesForUpdate();
	std::cout << "2" << std::endl;
	auto x = particles->getXData();
	auto f = particles->getFData();
	std::cout << "3" << std::endl;
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