#include "EulerIntegrator.h"

void EulerIntegrator::post_force()
{
	positionUpdate();
	velocityUpdate();
}