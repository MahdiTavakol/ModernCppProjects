#pragma once

#include "Ref.h"
#include "Engine.h"
#include <array>

class ForceField: public Ref {
public:
	ForceField(Engine& engine_) :
		Ref{ engine_,"1" }
	{}
	void init();
	void update();

protected:
	virtual void calculate_pair(std::array<double, 3>& dist_,
		double* fforce_, double& energy_) {}

};

