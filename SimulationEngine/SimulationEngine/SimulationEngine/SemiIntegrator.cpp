#include "SemiIntegrator.h"

SemiIntegrator::SemiIntegrator(std::vector<std::string> args_) :
	Integrator{ std::move(args_) }
{}

SemiIntegrator::SemiIntegrator() :
	Integrator{}
{}

void SemiIntegrator::post_force()
{
	Integrator::post_force();
	velocityUpdate();
	positionUpdate();
}