#pragma once

#include "Integrator.h"
#include "Engine.h"

// Semi-implicit Euler integration
class SemiIntegrator : public Integrator {
public:
	SemiIntegrator(std::vector<std::string> args_);
	SemiIntegrator();
	void post_force() override;
};
