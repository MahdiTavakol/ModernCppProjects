#include "Engine.h"
#include "Integrator.h"
#include "Particles.h"
#include "ForceField.h"
#include "Neighbor.h"
#include "Box.h"
#include "Fix.h"
#include "Error.h"


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
	error{std::make_unique<Error>(*this)}
{}

Engine::Engine() : 
	forcefield{std::make_unique<ForceField>(*this)},
	neighbor{ std::make_unique<Neighbor>(*this,0.0) }
{}

Engine::Engine(Run_Status& run_status_) :
	run_status{ run_status_ },
	forcefield{std::make_unique<ForceField>(*this)},
	neighbor{ std::make_unique<Neighbor>(*this,0.0) }
{}

Engine::~Engine() = default;

void Engine::setItem(std::unique_ptr<Ref>&& Ref_) {
	ItemRef iRef = detectRef(Ref_);

	switch (iRef) {
		case ItemRef::BOX:
			box.reset(dynamic_cast<Box*>(Ref_.release()));
			break;
		case ItemRef::PARTICLES:
			particles.reset(dynamic_cast<Particles*>(Ref_.release()));
			break;
		case ItemRef::INTEGRATOR:
			integrator.reset(dynamic_cast<Integrator*>(Ref_.release()));
			break;
		case ItemRef::FORCEFIELD:
			forcefield.reset(dynamic_cast<ForceField*>(Ref_.release()));
			break;
		case ItemRef::FIX:
			fixList.push_back(std::unique_ptr<Fix>(dynamic_cast<Fix*>(Ref_.release())));
			break;
		case ItemRef::NEIGHBOR:
			neighbor.reset(dynamic_cast<Neighbor*>(Ref_.release()));
			break;
		case ItemRef::ERROR:
			error.reset(dynamic_cast<Error*>(Ref_.release()));
			break;
		default:
			throw std::invalid_argument("The Ref is not recognized!");
	}	
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
vector<unique_ptr<Fix>>& Engine::getFixList() {
	return fixList;
}
std::unique_ptr<ForceField>& Engine::getForceField() {
	return forcefield;
}
unique_ptr<Fix>& Engine::returnFixById(string id_) {
	for (auto& fix : fixList) {
		if (fix->getId() == id_) {
			return fix;
		}
	}
	throw std::invalid_argument("The fix was not find!");
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

Engine::ItemRef Engine::detectRef(const std::unique_ptr<Ref>& Ref_)
{
	if (auto boxPtr = dynamic_cast<Box*>(Ref_.get())) {
		return ItemRef::BOX;
	}
	else if (auto intPtr = dynamic_cast<Integrator*>(Ref_.get())) {
		return ItemRef::INTEGRATOR;
	}
	else if (auto frcPtr = dynamic_cast<ForceField*>(Ref_.get())) {
		return ItemRef::FORCEFIELD;
	}
	else if (auto fixPtr = dynamic_cast<Fix*>(Ref_.get())) {
		return ItemRef::FIX;
	}
	else if (auto prtPtr = dynamic_cast<Particles*>(Ref_.get())) {
		return ItemRef::PARTICLES;
	}
	else if (auto nbrPtr = dynamic_cast<Neighbor*>(Ref_.get())) {
		return ItemRef::NEIGHBOR;
	}
	else if (auto errPtr = dynamic_cast<Error*>(Ref_.get())) {
		return ItemRef::ERROR;
	}
	else
		throw std::invalid_argument("The Ref is not recognized!");
}