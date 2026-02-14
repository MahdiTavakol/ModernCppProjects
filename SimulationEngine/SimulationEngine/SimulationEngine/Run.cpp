#include "Run.h"
#include "Integrator.h"

void Run::setup() {
	auto& integrator = engine().getIntegrator();
	integrator->setDt(dt);
}
void Run::start(int nSteps_, int timestep_ ) {
	auto& integrator = engine().getIntegrator();
	integrator->init();
	integrator->setup();
	nSteps = nSteps_;
	for (currentStep = timestep_; currentStep < nSteps; currentStep++) {
		integrator->initial_integrate();
		integrator->pre_force();
		integrator->post_force();
		integrator->final_integrate();
	}
}