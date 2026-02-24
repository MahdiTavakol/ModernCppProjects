#pragma once

#include "Ref.h"
#include "Engine.h"

class Run : public Ref {
public:
	Run(Engine& engine_);
	Run(Engine& engine_, std::vector<std::string>& args_);
	Run(Engine& engine_, double dt_);
	void setup();
	void start(int nSteps_, int timestep_ = 0);
	int getSteps();

private:
	int nSteps = 0;
	int currentStep = 0;
};