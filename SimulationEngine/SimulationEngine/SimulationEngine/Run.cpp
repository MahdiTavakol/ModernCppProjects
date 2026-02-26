#include "Run.h"
#include "Integrator.h"
#include "Error.h"
#include "Engine.h"
#include "FixList.h"
#include "ForceField.h"
#include "Neighbor.h"
#include <iostream>

Run::Run(Engine& engine_) :
	Ref{ engine_,"1" }
{}

Run::Run(Engine& engine_, std::vector<std::string>& args_) :
	Ref{ engine_,"1" }
{
	int nargs = args_.size();
	if (nargs < 2) {
		engine().getError()->one("Not enough arguments for run command! Expected at least 2, got " + std::to_string(nargs));
		nSteps = 0;
		return;
	}
	nSteps = std::stoi(args_[1]);
}

Run::Run(Engine& engine_, double dt_) :
	Ref{ engine_,"1" }
{}

void Run::setup() {
	auto& integrator = engine().getIntegrator();
	auto& err = engine().getError();
	auto& particles = engine().getParticlesForUpdate();
}
void Run::start(int nSteps_, int timestep_ ) {
	auto& integrator = engine().getIntegrator();
	if (integrator == nullptr)
		std::cout << "The integrator is nullptr" << std::endl;
	auto& run_status = engine().getStatus();
	auto& fixList = engine().getFixListObj();
	if (fixList == nullptr)
		std::cout << "The fixList is nullptr" << std::endl;
	auto& neighbor = engine().getNeighbor();
	if (neighbor == nullptr)
		std::cout << "The neighbor is nullptr" << std::endl;
	auto& forcefield = engine().getForceField();
	if (forcefield == nullptr)
		std::cout << "The forcefield is nullptr" << std::endl;
	if (run_status == Engine::Run_Status::VERBOSE)
		std::cout << "Initialization" << std::endl;
	forcefield->init();
	neighbor->init();
	fixList->init();
	//integrator->init();
	if (run_status == Engine::Run_Status::VERBOSE)
		std::cout << "Setting up" << std::endl;
	fixList->setup();
	//integrator->setup();
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