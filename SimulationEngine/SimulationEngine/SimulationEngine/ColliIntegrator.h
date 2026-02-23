#pragma once

#include "Integrator.h"
#include "Engine.h"

#include <vector>

// forward Euler integration
class ColliIntegrator : public Integrator {
public:
	ColliIntegrator(Engine& engine_, std::vector<std::string> args_);
	ColliIntegrator(Engine& engine);
	virtual void positionUpdate() override;
	void post_force() override;

private:
	// delta velocity vector
	std::vector<double> dv;
	// the vector capacity (reserve)
	int nmax;
	// the number of atoms in the vector (push_back)
	int nlocal;
	// checking if anything has collided
	void deltaVUpdate();
	// adding deltaV
	void velocityUpdate_2();
};
