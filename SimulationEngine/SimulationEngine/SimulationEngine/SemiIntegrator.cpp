#include "SemiIntegrator.h"

SemiIntegrator::SemiIntegrator(Engine& engine_,
	std::vector<std::string> args_) :
	Integrator{ engine_, std::move(args_) }
{}

SemiIntegrator::SemiIntegrator(Engine& engine) :
	Integrator{ engine }
{}

void SemiIntegrator::post_force()
{
	Integrator::post_force();
	velocityUpdate();
	positionUpdate();
}