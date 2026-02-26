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
	forcefield{ std::make_unique<ForceField>(*this)},
	neighbor{ std::make_unique<Neighbor>(*this,0.0)},
	error{std::make_unique<Error>(*this)},
	fixListObj{std::make_unique<FixList>(*this)}
{}

Engine::Engine() : 
	forcefield{std::make_unique<ForceField>(*this)},
	neighbor{ std::make_unique<Neighbor>(*this,0.0) },
	fixListObj{std::make_unique<FixList>(*this)}
{}

Engine::Engine(Run_Status& run_status_) :
	run_status{ run_status_ },
	forcefield{std::make_unique<ForceField>(*this)},
	neighbor{ std::make_unique<Neighbor>(*this,0.0) }
{}

Engine::~Engine() = default;

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
		fixListObj.reset(dynamic_cast<FixList*>(Ref_.release()));
		return;
	}
	else if (auto fixPtr = dynamic_cast<Fix*>(Ref_.get())) {
		// adding the fix
		if (fixListObj == nullptr)
			fixListObj = std::make_unique<FixList>(*this);
		fixListObj->addFix(std::move(std::unique_ptr<Fix>(dynamic_cast<Fix*>(Ref_.release()))));
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
std::unique_ptr<FixList>& Engine::getFixListObj()
{
	return fixListObj;
}
std::unique_ptr<ForceField>& Engine::getForceField() {
	return forcefield;
}
unique_ptr<Fix>& Engine::returnFixById(string id_) {
	return fixListObj->returnFixById(id_);
	/*
	for (auto& fix : fixList) {
		if (fix->getId() == id_) {
			return fix;
		}
	}
	std::string errorMessage = "The fix with id " + id_ + " was not found!";
	throw std::invalid_argument(errorMessage.c_str());
	*/
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

std::unique_ptr<Neighbor>& Engine::getNeighbor() {
	return neighbor;
}

std::unique_ptr<Error>& Engine::getError() {
	return error;
}

std::unique_ptr<Run>& Engine::getRunCommand() {
	return run;
}

