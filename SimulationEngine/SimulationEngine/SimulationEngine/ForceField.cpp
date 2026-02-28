#include "ForceField.h"
#include "Particles.h"
#include "Error.h"
#include <iostream>
#include <stdexcept>

ForceField::ForceField(std::vector<std::string>& args_) :
	Ref{  "1" }
{
	auto nargs = args_.size();
	if (nargs < 2) {
		throw std::invalid_argument("Not enough arguments for forcefield command! Expected at least 2, got " + std::to_string(nargs));
		return;
	}
}

ForceField::ForceField() :
	Ref{"1" }
{}

void ForceField::injectDependencies(Engine& engine_) {
	Ref::injectDependencies(engine_);
	box = engine_.getBox().get();
	particles = engine_.getParticlesForUpdate().get();
	checkPointer(particles, "box");
	checkPointer(particles, "particles");
}


void ForceField::init() {
	return;
	auto f = particles->getFData();
	particles->getNmaxNlocal(nmax, nlocal);

	for (int i = 0; i < nlocal; i++) {
		f[3 * i] = 0.0;
		f[3 * i + 1] = 0.0;
		f[3 * i + 2] = 0.0;
	}
}

void ForceField::resetForce()
{
	particles->getNmaxNlocal(nmax, nlocal);
	auto f = particles->getFData();
	std::fill_n(f, 3 * nlocal, 0.0);
}

void ForceField::update() {

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

FixedForce::FixedForce(std::vector<std::string>& args_):
	ForceField(args_)
{
	// forcefield fixed fx fy fz
	auto nargs = args_.size();
	if (nargs != 5)
		throw std::invalid_argument("Not enough args in the Fixed ForceField!");

	fx = std::stoi(args_[2]);
	fy = std::stoi(args_[3]);
	fz = std::stoi(args_[4]);
}

void FixedForce::update()
{
	for (int i = 0; i < nlocal; i++) {
		particles->F(i, 0) = fx;
		particles->F(i, 1) = fy;
		particles->F(i, 2) = fz;
	}
}