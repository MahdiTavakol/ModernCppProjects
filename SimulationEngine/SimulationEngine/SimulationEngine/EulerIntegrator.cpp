#include "EulerIntegrator.h"

EulerIntegrator::EulerIntegrator(std::vector<std::string> args_):
	Integrator{args_ }
{}

EulerIntegrator::EulerIntegrator() :
		Integrator{}
{}

void EulerIntegrator::post_force()
{
	Integrator::post_force();
	positionUpdate();
	velocityUpdate();
}