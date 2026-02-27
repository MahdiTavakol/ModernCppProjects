#include "Integrator.h"
#include "Particles.h"
#include "Fix.h"
#include "ForceField.h"
#include "Neighbor.h"
#include "Engine.h"
#include "Error.h"

#include <iostream>
#include <stdexcept>

Integrator::Integrator(std::vector<std::string> args_) :
	Ref{ "1" }
{
	auto nargs = args_.size();
	if (nargs < 3) {
		throw std::invalid_argument("Not enough arguments for integrator command! Expected at least 2, got " + std::to_string(nargs));
		return;
	}
	dt = std::stod(args_[2]);
}

Integrator::Integrator() :
	Ref{"1" }
{}
Integrator::Integrator(double dt_) :
	Ref{ "1" },
	dt{ dt_ }
{}

void Integrator::injectDependencies(Engine& engine_) {
	Ref::injectDependencies(engine_);
	particles = engine_.getParticlesForUpdate().get();
	checkPointer(particles, "particles");
}

void Integrator::setDt(double dt_) { dt = dt_; }


void Integrator::post_force() {
}


void Integrator::velocityUpdate()
{
	int nlocal, nmax;
	particles->getNmaxNlocal(nmax, nlocal);

	for (int i = 0; i < nlocal; i++)
		for (int j = 0; j < 3; j++)
		{
			double a = particles->F(i ,j) / particles->M(i);
			particles->V(i , j) += dt * a;
		}
}
void Integrator::positionUpdate()
{
	int nlocal, nmax;
	particles->getNmaxNlocal(nmax, nlocal);

	for (int i = 0; i < nlocal; i++)
		for (int j = 0; j < 3; j++)
		{
			particles->X(i, j) += dt * particles->V(i, j);
		}
}