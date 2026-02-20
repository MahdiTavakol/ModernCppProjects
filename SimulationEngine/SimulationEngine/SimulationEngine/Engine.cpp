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

void Engine::resetBox(unique_ptr<Box>& box_) {
	box = std::move(box_);
}
void Engine::resetParticles(unique_ptr<Particles> particles_) {
	particles = std::move(particles_);
}
void Engine::registerItem(std::unique_ptr<Type>&& type_) {

	if (auto boxPtr = dynamic_cast<Box*>(type_.get())) {
		box.reset(boxPtr);
	}
	else if (auto intPtr = dynamic_cast<Integrator*>(type_.get())) {
		integrator.reset(intPtr);
	} 
	else if (auto frcPtr = dynamic_cast<ForceField*>(type_.get())) {
		forcefield.reset(frcPtr);
	}
	else if (auto fixPtr = dynamic_cast<Fix*>(type_.get())) {
		std::unique_ptr<Fix> fixUniquePtr;
		fixUniquePtr.reset(fixPtr);
		fixList.push_back(std::move(fixUniquePtr));
	}
	else if (auto prtPtr = dynamic_cast<Particles*>(type_.get())) {
		particles.reset(prtPtr);
	}
	else if (auto nbrPtr = dynamic_cast<Neighbor*>(type_.get())) {
		neighbor.reset(nbrPtr);
	}
	else 
		throw std::invalid_argument("The type is not recognized!");

	type_.release();

	
}

void Engine::registerBox(unique_ptr<Box>& box_)
{
	box = std::move(box_);
}
void Engine::registerParticles(unique_ptr<Particles>& particles_)
{
	particles = std::move(particles_);
}
void Engine::registerIntegrator(unique_ptr<Integrator>& integrator_)
{
	integrator = std::move(integrator_);
}
void Engine::registerForceField(std::unique_ptr<ForceField>& forcefield_)
{
	forcefield = std::move(forcefield_);
}
void Engine::registerFix(unique_ptr<Fix> fix_)
{
	fixList.push_back(std::move(fix_));
}
void Engine::registerNeighbor(unique_ptr<Neighbor>& neighbor_)
{
	neighbor = std::move(neighbor_);
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