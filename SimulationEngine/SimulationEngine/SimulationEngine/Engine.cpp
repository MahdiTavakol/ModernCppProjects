#include "Engine.h"
#include "Integrator.h"
#include "Particles.h"
#include "ForceField.h"
#include "Neighbor.h"
#include "Box.h"
#include "Fix.h"
#include "FixList.h"
#include "Error.h"
#include "Run.h"


#include <utility>
#include <stdexcept>

using std::make_unique;
using std::unique_ptr;
using std::vector;
using std::string;

Engine::Engine(unique_ptr<Box>& box_,
	unique_ptr<Particles>& particles_) :
	box{ std::move(box_) },
	particles{ std::move(particles_) },
	forcefield{ std::make_unique<ForceField>()},
	neighbor{ std::make_unique<Neighbor>(0.0)},
	error{std::make_unique<Error>()},
	fixList{std::make_unique<FixList>()}
{}

Engine::Engine() : 
	forcefield{std::make_unique<ForceField>()},
	neighbor{ std::make_unique<Neighbor>(0.0) },
	fixList{std::make_unique<FixList>()},
	particles{std::make_unique<Particles>(0)},
	error{std::make_unique<Error>()}
{}

Engine::Engine(Run_Status& run_status_) :
	run_status{ run_status_ },
	forcefield{std::make_unique<ForceField>()},
	neighbor{ std::make_unique<Neighbor>(0.0) }
{}

Engine::~Engine() = default;

// injecting dependencies
void Engine::injectDependencies() {
	if (integrator)
		integrator->injectDependencies(*this);
	if (particles)
		particles->injectDependencies(*this);
	if (forcefield)
		forcefield->injectDependencies(*this);
	if (fixList)
		fixList->injectDependencies(*this);
	if (neighbor)
		neighbor->injectDependencies(*this);
	if (box)
		box->injectDependencies(*this);
	if (run)
		run->injectDependencies(*this);
}

void Engine::init() {
	if (integrator)
		integrator->init();
	if (particles)
		particles->init();
	if (forcefield)
		forcefield->init();
	if (fixList)
		fixList->init();
	if (neighbor)
		neighbor->init();
	if (box)
		box->init();
	if (run)
		run->init();
}

void Engine::resetError(std::unique_ptr<Error>&& error_) {
	error = std::move(error_);
	// since the error has changed the dependencies must be reinjected.
	injectDependencies();
}

void Engine::resetParticles(std::unique_ptr<Particles>&& particles_) {
	particles = std::move(particles_);
	// since the particles has changed we need to reinject the dependencies
	injectDependencies();
}

void Engine::setItem(std::unique_ptr<Ref>&& Ref_) {
	if (auto boxPtr = dynamic_cast<Box*>(Ref_.get())) {
		box.reset(dynamic_cast<Box*>(Ref_.release()));
		return;
	}
	else if (auto intPtr = dynamic_cast<Integrator*>(Ref_.get())) {
		integrator.reset(dynamic_cast<Integrator*>(Ref_.release()));
		return;
	}
	else if (auto frcPtr = dynamic_cast<ForceField*>(Ref_.get())) {
		forcefield.reset(dynamic_cast<ForceField*>(Ref_.release()));
		return;
	}
	else if (auto fixListPtr = dynamic_cast<FixList*>(Ref_.get()))
	{
		fixList.reset(dynamic_cast<FixList*>(Ref_.release()));
		return;
	}
	else if (auto fixPtr = dynamic_cast<Fix*>(Ref_.get())) {
		// adding the fix
		if (fixList == nullptr)
			fixList = std::make_unique<FixList>();
		fixList->addFix(std::move(std::unique_ptr<Fix>(dynamic_cast<Fix*>(Ref_.release()))));
		return;
	}
	else if (auto prtPtr = dynamic_cast<Particles*>(Ref_.get())) {
		particles.reset(dynamic_cast<Particles*>(Ref_.release()));
		return;
	}
	else if (auto nbrPtr = dynamic_cast<Neighbor*>(Ref_.get())) {
		neighbor.reset(dynamic_cast<Neighbor*>(Ref_.release()));
		return;
	}
	else if (auto errPtr = dynamic_cast<Error*>(Ref_.get())) {
		error.reset(dynamic_cast<Error*>(Ref_.release()));
		return;
	}
	else if (auto runPtr = dynamic_cast<Run*>(Ref_.get())) {
		run.reset(dynamic_cast<Run*>(Ref_.release()));
		return;
	}
	else
		throw std::invalid_argument("The Ref is not recognized!");
}

Ref* Engine::getItem(ItemRef Ref_) {
	switch (Ref_) {
		case ItemRef::BOX:
			return box.get();
		case ItemRef::PARTICLES:
			return particles.get();
		case ItemRef::INTEGRATOR:
			return integrator.get();
		case ItemRef::FORCEFIELD:
			return forcefield.get();
		case ItemRef::FIX:
			throw std::invalid_argument("There can be multiple fixes, please use getFixList() to get the list of fixes!");
		case ItemRef::NEIGHBOR:
			return neighbor.get();
		case ItemRef::ERROR:
			return error.get();
		case ItemRef::RUN:
			return run.get();
		default:
			throw std::invalid_argument("The Ref is not recognized!");
	}
}
const unique_ptr<Box>& Engine::getBox() const {
	return box;
}
const unique_ptr<Particles>& Engine::getParticles() const {
	return particles;
}
unique_ptr<Particles>& Engine::getParticlesForUpdate() {
	return particles;
}
unique_ptr<Integrator>& Engine::getIntegrator() {
	return integrator;
}
std::unique_ptr<FixList>& Engine::getFixList()
{
	return fixList;
}
std::unique_ptr<ForceField>& Engine::getForceField() {
	return forcefield;
}
unique_ptr<Fix>& Engine::returnFixById(string id_) {
	return fixList->returnFixById(id_);
}

void Engine::setStatus(const std::string newStatus_) {
	if (newStatus_ == "silent") {
		run_status = Run_Status::SILENT;
	}
	else if (newStatus_ == "verbose") {
		run_status = Run_Status::VERBOSE;
	}
	else {
		throw std::invalid_argument("The run status is not recognized!");
	}
}

const Engine::Run_Status& Engine::getStatus() const {
	return run_status;
}

Engine::Run_Status Engine::getStatusVal() const {
	return run_status;
}

std::unique_ptr<Neighbor>& Engine::getNeighbor() {
	return neighbor;
}

std::unique_ptr<Error>& Engine::getError() {
	return error;
}

std::unique_ptr<Run>& Engine::getRunCommand() {
	return run;
}

