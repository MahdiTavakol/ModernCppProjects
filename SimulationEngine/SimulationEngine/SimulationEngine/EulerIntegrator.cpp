#include "EulerIntegrator.h"

EulerIntegrator::EulerIntegrator(Engine& engine_, std::vector<std::string> args_):
	Integrator{ engine_, args_ }
{}

EulerIntegrator::EulerIntegrator(Engine& engine) :
		Integrator{ engine }
{}

void EulerIntegrator::post_force()
{
	Integrator::post_force();
	positionUpdate();
	velocityUpdate();
}