#pragma once

#include "Ref.h"
#include "Engine.h"

class Run : public Ref {
public:
	Run();
	Run(std::vector<std::string>& args_);
	Run(double dt_);
	virtual void injectDependencies(Engine& engine_) override;
	void setup();
	void start(int nSteps_, int timestep_ = 0);
	void start() {
		start(nSteps, 0);
	}
	int getSteps();

private:
	// reference to the integrator
	Integrator* integrator;
	// reference to the fixList
	FixList* fixList;
	// reference to the neighbor
	Neighbor* neighbor;
	// reference to the forcefield
	ForceField* forcefield;
	// run_status
	Engine::Run_Status run_status;
	// simulation settings
	int nSteps = 0;
	int currentStep = 0;
};