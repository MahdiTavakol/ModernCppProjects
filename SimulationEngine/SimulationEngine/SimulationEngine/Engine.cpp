#include "Engine.h"
#include "Integrator.h"
#include "Particles.h"
#include "ForceField.h"
#include "Neighbor.h"
#include "Box.h"
#include "Fix.h"


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
	neighbor{ std::make_unique<Neighbor>(*this,0.0)}
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

void Engine::setItem(std::unique_ptr<Type>&& type_) {
	ItemType iType = detectType(type_);

	switch (iType) {
		case ItemType::BOX:
			box.reset(dynamic_cast<Box*>(type_.release()));
			break;
		case ItemType::PARTICLES:
			particles.reset(dynamic_cast<Particles*>(type_.release()));
			break;
		case ItemType::INTEGRATOR:
			integrator.reset(dynamic_cast<Integrator*>(type_.release()));
			break;
		case ItemType::FORCEFIELD:
			forcefield.reset(dynamic_cast<ForceField*>(type_.release()));
			break;
		case ItemType::FIX:
			fixList.push_back(std::unique_ptr<Fix>(dynamic_cast<Fix*>(type_.release())));
			break;
		case ItemType::NEIGHBOR:
			neighbor.reset(dynamic_cast<Neighbor*>(type_.release()));
			break;
		default:
			throw std::invalid_argument("The type is not recognized!");
	}	
}

Type* Engine::getItem(ItemType type_) {
	switch (type_) {
		case ItemType::BOX:
			return box.get();
		case ItemType::PARTICLES:
			return particles.get();
		case ItemType::INTEGRATOR:
			return integrator.get();
		case ItemType::FORCEFIELD:
			return forcefield.get();
		case ItemType::FIX:
			throw std::invalid_argument("There can be multiple fixes, please use getFixList() to get the list of fixes!");
		case ItemType::NEIGHBOR:
			return neighbor.get();
		default:
			throw std::invalid_argument("The type is not recognized!");
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

Engine::ItemType Engine::detectType(const std::unique_ptr<Type>& type_)
{
	if (auto boxPtr = dynamic_cast<Box*>(type_.get())) {
		return ItemType::BOX;
	}
	else if (auto intPtr = dynamic_cast<Integrator*>(type_.get())) {
		return ItemType::INTEGRATOR;
	}
	else if (auto frcPtr = dynamic_cast<ForceField*>(type_.get())) {
		return ItemType::FORCEFIELD;
	}
	else if (auto fixPtr = dynamic_cast<Fix*>(type_.get())) {
		return ItemType::FIX;
	}
	else if (auto prtPtr = dynamic_cast<Particles*>(type_.get())) {
		return ItemType::PARTICLES;
	}
	else if (auto nbrPtr = dynamic_cast<Neighbor*>(type_.get())) {
		return ItemType::NEIGHBOR;
	}
	else
		throw std::invalid_argument("The type is not recognized!");
}