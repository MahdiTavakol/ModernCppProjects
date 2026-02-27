#include "Run.h"
#include "Integrator.h"
#include "Error.h"
#include "Engine.h"
#include "FixList.h"
#include "ForceField.h"
#include "Neighbor.h"
#include <iostream>

Run::Run() :
	Ref{"1" }
{}

Run::Run( std::vector<std::string>& args_) :
	Ref{ "1" }
{
	int nargs = args_.size();
	if (nargs < 2) {
		throw std::invalid_argument("Not enough arguments for run command! Expected at least 2, got " + std::to_string(nargs));
		nSteps = 0;
		return;
	}
	nSteps = std::stoi(args_[1]);
}

Run::Run(double dt_) :
	Ref{"1" }
{}

void Run::injectDependencies(Engine& engine_) {
	Ref::injectDependencies(engine_);
	integrator = engine_.getIntegrator().get();
	fixList = engine_.getFixList().get();
	neighbor = engine_.getNeighbor().get();
	forcefield = engine_.getForceField().get();
	run_status = engine_.getStatusVal();
	checkPointer(integrator, "integrator");
	checkPointer(fixList, "fixList");
	checkPointer(neighbor, "neighbor");
	checkPointer(forcefield, "forcefield");
}

void Run::setup() {
}

void Run::start(int nSteps_, int timestep_ ) {
	if (forcefield == nullptr)
		std::cout << "forcefield" << std::endl;
	if (neighbor == nullptr)
		std::cout << "neighbor" << std::endl;
	if (fixList == nullptr)
		std::cout << "fixlist" << std::endl;
	if (integrator == nullptr)
		std::cout << "integrator" << std::endl;
	if (run_status == Engine::Run_Status::VERBOSE)
		std::cout << "Initialization" << std::endl;
	forcefield->init();
	neighbor->init();
	fixList->init();
	if (run_status == Engine::Run_Status::VERBOSE)
		std::cout << "Setting up" << std::endl;
	fixList->setup();
	nSteps = nSteps_;
	if (run_status == Engine::Run_Status::VERBOSE)
		std::cout << "Running for " << nSteps << " steps" << std::endl;
	for (currentStep = timestep_; currentStep < nSteps; currentStep++) {
		if (run_status == Engine::Run_Status::VERBOSE)
			std::cout << "Step " << currentStep << std::endl;
		fixList->initial_integrate();
		fixList->pre_force();
		neighbor->update();
		forcefield->update();
		fixList->post_force();
		integrator->post_force();
		fixList->final_integrate();
	}
}

int Run::getSteps() {
	return nSteps;
}