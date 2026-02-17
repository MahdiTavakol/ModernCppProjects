#include "EulerIntegrator.h"

void EulerIntegrator::post_force()
{
	Integrator::post_force();
	positionUpdate();
	velocityUpdate();
}