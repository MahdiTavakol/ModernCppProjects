#pragma once

#include "Integrator.h"
#include "Engine.h"

// Semi-implicit Euler integration
class SemiIntegrator : public Integrator {
public:
	SemiIntegrator(Engine& engine) :
		Integrator{ engine }
	{}
	void post_force() override;
};
