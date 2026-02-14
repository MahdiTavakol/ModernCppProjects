#pragma once

#include "Type.h"
#include "Engine.h"

class Run : public Type {
public:
	Run(Engine& engine_) :
		Type{ engine_,"1" },
		dt{ 1.0 }
	{}
	Run(Engine& engine_, double dt_) :
		Type{ engine_,"1" },
		dt{ 1.0 }
	{}
	void setup();
	void start(int nSteps_, int timestep_ = 0);

private:
	double dt;
	int nSteps;
	int currentStep = 0;
};