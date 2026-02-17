#include "SemiIntegrator.h"

void SemiIntegrator::post_force()
{
	Integrator::post_force();
	velocityUpdate();
	positionUpdate();
}