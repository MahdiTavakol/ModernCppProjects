#include "Integrator.h"
#include "Particles.h"
#include "Fix.h"
#include "ForceField.h"
#include "Neighbor.h"

#include <iostream>

void Integrator::init() {
	auto& neighbor = engine().getNeighbor();
	neighbor->init();
	auto& forcefield = engine().getForceField();
	forcefield->init();
  	auto& fixList = engine().getFixList();
	for (auto& fix : fixList) {
		fix->init();
	}
}

void Integrator::setup() {
	auto& fixList = engine().getFixList();
	for (auto& fix : fixList) {
		fix->setup();
	}
}

void Integrator::neighbor_build() {
	auto& neighbor = engine().getNeighbor();
	neighbor->update();
}

void Integrator::initial_integrate() {
	auto& fixList = engine().getFixList();
	for (auto& fix : fixList) {
		if (fix->mask & INIT_INTEGRATE)
			fix->initial_integrate();
	}
}

void Integrator::pre_force() {
	auto& fixList = engine().getFixList();
	for (auto& fix : fixList) {
		if (fix->mask & PRE_FORCE)
			fix->pre_force();
	}
}

void Integrator::force() {
 	auto& forcefield = engine().getForceField();
	forcefield->update();
}

void Integrator::post_force() {
 	auto& fixList = engine().getFixList();
	for (auto& fix : fixList) {
		if (fix->mask & POST_FORCE)
			fix->post_force();
	}
}

void Integrator::final_integrate() {
	auto& fixList = engine().getFixList();
	for (auto& fix : fixList) {
		if (fix->mask & FINAL_INTEGRATE)
			fix->final_integrate();
	}
}

void Integrator::velocityUpdate()
{
	int nlocal, nmax;
	auto& particles = engine().getParticlesForUpdate();
	auto v = particles->getVData();
	auto f = particles->getFData();
	auto m = particles->getMData();
	particles->getNmaxNlocal(nmax, nlocal);

	for (int i = 0; i < nlocal; i++)
		for (int j = 0; j < 3; j++)
		{
			double a = f[3 * i + j] / m[i];
			v[3 * i + j] += dt * a;
		}
}
void Integrator::positionUpdate()
{
	int nlocal, nmax;
	auto& particles = engine().getParticlesForUpdate();
	auto x = particles->getXData();
	auto v = particles->getVData();
	particles->getNmaxNlocal(nmax, nlocal);

	for (int i = 0; i < nlocal; i++)
		for (int j = 0; j < 3; j++)
		{
			x[3 * i + j] += dt * v[3 * i + j];
		}
}