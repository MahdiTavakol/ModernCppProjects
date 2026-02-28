#pragma once

#include "Ref.h"
#include "Engine.h"
#include <array>

struct Connection
{
	int atom1id;
	int atom2id;
	double springCoeff;
};


class ForceField: public Ref {
public:
	ForceField( std::vector<std::string>& args_);
	ForceField();
	virtual void injectDependencies(Engine& engine_) override;
	virtual void init();
	virtual void resetForce();
	virtual void update();

protected:
	// box
	Box* box = nullptr;
	// particles
	Particles* particles = nullptr;
	// number of particles and maximum number of them
	int nlocal, nmax;
	virtual void calculate_pair(std::array<double, 3>& dist_,
		double* fforce_, double& energy_) {}

};

class FixedForce : public ForceField {
public:
	FixedForce(std::vector<std::string>& args_);
	void update() override;

protected:
	double fx, fy, fz;

};


