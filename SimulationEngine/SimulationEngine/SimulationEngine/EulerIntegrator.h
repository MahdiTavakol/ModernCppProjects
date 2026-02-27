#pragma once

#include "Integrator.h"
#include "Engine.h"

// forward Euler integration
class EulerIntegrator : public Integrator {
public:
	EulerIntegrator(std::vector<std::string> args_);
	EulerIntegrator();
	void post_force() override;
};
