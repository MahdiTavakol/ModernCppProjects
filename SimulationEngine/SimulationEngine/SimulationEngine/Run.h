#pragma once

#include "Ref.h"
#include "Engine.h"

class Run : public Ref {
public:
	Run(Engine& engine_) :
		Ref{ engine_,"1" },
		dt{ 1.0 }
	{}
	Run(Engine& engine_, double dt_) :
		Ref{ engine_,"1" },
		dt{ 1.0 }
	{}
	void setup();
	void start(int nSteps_, int timestep_ = 0);

private:
	double dt;
	int nSteps;
	int currentStep = 0;
};