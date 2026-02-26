#include "ForceField.h"
#include "Particles.h"
#include "Error.h"
#include <iostream>

ForceField::ForceField(Engine& engine_, std::vector<std::string> args_) :
	Ref{ engine_, "1" }
{
	auto nargs = args_.size();
	auto& error = engine().getError();
	if (nargs < 2) {
		error->one("Not enough arguments for forcefield command! Expected at least 2, got " + std::to_string(nargs));
		return;
	}
}

ForceField::ForceField(Engine& engine_) :
	Ref{ engine_,"1" }
{}

void ForceField::injectDependencies(Engine& engine_) {
	Ref::injectDependencies(engine_);
	box = engine_.getBox().get();
	particles = engine_.getParticlesForUpdate().get();
}


void ForceField::init() {
	int nlocal, nmax;
	return;
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
	particles->getNmaxNlocal(nmax, nlocal);

	double energy;
	for (int i = 0; i < nlocal; i++)
		for (int j = 0; j < nlocal; j++) {
			if (j == i) continue;
			std::array<double, 3> dist = {
				particles->X(i,0)- particles->X(j,0),
				particles->X(i,1)- particles->X(j,1),
				particles->X(i,2)- particles->X(j,2)
			};
			calculate_pair(dist, &(particles->F(i,0)), energy);
		}
}