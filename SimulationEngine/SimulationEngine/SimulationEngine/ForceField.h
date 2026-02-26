#pragma once

#include "Ref.h"
#include "Engine.h"
#include <array>

class ForceField: public Ref {
public:
	ForceField(Engine& engine_, std::vector<std::string> args_);
	ForceField(Engine& engine_);
	virtual void injectDependencies(Engine& engine_) override;
	virtual void init();
	virtual void update();

protected:
	// box
	Box* box = nullptr;
	// particles
	Particles* particles = nullptr;
	virtual void calculate_pair(std::array<double, 3>& dist_,
		double* fforce_, double& energy_) {}

};

