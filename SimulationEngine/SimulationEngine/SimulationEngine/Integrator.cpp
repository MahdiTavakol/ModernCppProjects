#include "Integrator.h"
#include "Particles.h"
#include "Fix.h"
#include "ForceField.h"
#include "Neighbor.h"
#include "Engine.h"
#include "Error.h"

#include <iostream>

Integrator::Integrator(Engine& engine_,
	std::vector<std::string> args_) :
	Ref{ engine_, "1" }
{
	auto nargs = args_.size();
	if (nargs < 3) {
		engine().getError()->one("Not enough arguments for integrator command! Expected at least 2, got " + std::to_string(nargs));
		return;
	}
	dt = std::stod(args_[2]);
}

Integrator::Integrator(Engine& engine_) :
	Ref{ engine_,"1" }
{}
Integrator::Integrator(Engine& engine_, double dt_) :
	Ref{ engine_,"1" },
	dt{ dt_ }
{}

void Integrator::injectDependencies() {
	particles = engine().getParticlesForUpdate().get();
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