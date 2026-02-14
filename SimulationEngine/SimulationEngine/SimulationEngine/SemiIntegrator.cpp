#include "SemiIntegrator.h"

void SemiIntegrator::post_force()
{
	velocityUpdate();
	positionUpdate();
}