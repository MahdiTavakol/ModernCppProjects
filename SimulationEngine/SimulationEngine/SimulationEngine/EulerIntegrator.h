#pragma once

#include "Integrator.h"
#include "Engine.h"

// forward Euler integration
class EulerIntegrator : public Integrator {
public:
	EulerIntegrator(Engine& engine) :
		Integrator{ engine }
	{}
	void post_force() override;
};
