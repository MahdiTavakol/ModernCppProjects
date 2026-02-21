#pragma once

#include "Integrator.h"
#include "Engine.h"

// Semi-implicit Euler integration
class SemiIntegrator : public Integrator {
public:
	SemiIntegrator(Engine& engine_,
		std::vector<std::string> args_);
	SemiIntegrator(Engine& engine);
	void post_force() override;
};
