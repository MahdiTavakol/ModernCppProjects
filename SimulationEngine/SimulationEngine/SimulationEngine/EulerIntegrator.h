#pragma once

#include "Integrator.h"
#include "Engine.h"

// forward Euler integration
class EulerIntegrator : public Integrator {
public:
	EulerIntegrator(Engine& engine_, std::vector<std::string> args_);
	EulerIntegrator(Engine& engine);
	void post_force() override;
};
