#include "Engine.h"
#include "Integrator.h"
#include "Particles.h"
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
	particles{ std::move(particles_) }
{}

Engine::Engine() = default;
Engine::~Engine() = default;

void Engine::resetBox(unique_ptr<Box>& box_) {
	box = std::move(box_);
}
void Engine::resetParticles(unique_ptr<Particles> particles_) {
	particles = std::move(particles_);
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
void Engine::registerFix(unique_ptr<Fix> fix_)
{
	fixList.push_back(std::move(fix_));
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
unique_ptr<Fix>& Engine::returnFixById(string id_) {
	/*for (int i = 0; i < fixList.size(); i++)
	{
		if (fixList[i]->getId() == id_) {
			return fixList[i];
		}
	}*/
	for (auto& fix : fixList) {
		if (fix->getId() == id_) {
			return fix;
		}
	}
	throw std::invalid_argument("The fix was not find!");
}