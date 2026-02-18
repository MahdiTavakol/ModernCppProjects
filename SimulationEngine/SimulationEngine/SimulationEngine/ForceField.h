#pragma once

#include "Type.h"
#include "Engine.h"
#include <array>

class ForceField: public Type {
public:
	ForceField(Engine& engine_) :
		Type{ engine_,"1" }
	{}
	void update();

protected:
	virtual void calculate_pair(std::array<double,3>& dist_,
		double* fforce_, double& energy_) = 0;

};

