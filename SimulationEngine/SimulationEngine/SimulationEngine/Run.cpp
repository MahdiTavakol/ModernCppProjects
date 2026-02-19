#include "Run.h"
#include "Integrator.h"
#include <iostream>

void Run::setup() {
	auto& integrator = engine().getIntegrator();
	integrator->setDt(dt);
}
void Run::start(int nSteps_, int timestep_ ) {
	auto& integrator = engine().getIntegrator();
	auto& run_status = engine().getStatus();
	if (run_status == Engine::Run_Status::VERBOSE)
		std::cout << "Initialization" << std::endl;
	integrator->init();
	if (run_status == Engine::Run_Status::VERBOSE)
		std::cout << "Setting up" << std::endl;
	integrator->setup();
	nSteps = nSteps_;
	if (run_status == Engine::Run_Status::VERBOSE)
		std::cout << "Running for " << nSteps << " steps" << std::endl;
	for (currentStep = timestep_; currentStep < nSteps; currentStep++) {
		if (run_status == Engine::Run_Status::VERBOSE)
			std::cout << "Step " << currentStep << std::endl;
		integrator->initial_integrate();
		integrator->pre_force();
		integrator->force();
		integrator->post_force();
		integrator->final_integrate();
	}
}