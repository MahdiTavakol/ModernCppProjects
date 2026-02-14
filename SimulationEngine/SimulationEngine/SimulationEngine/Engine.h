#pragma once

#include <memory>
#include <vector>

class Box;
class Particles;
class Integrator;
class Fix;

using std::make_unique, std::unique_ptr, std::vector;

class Engine {
public:
	static enum class ItemType { BOX, PARTICLES };

	// constructor with prebuilt types
	Engine(unique_ptr<Box>& box_,
		unique_ptr<Particles>& particles_);
	// default constructor to be used in unittests
	Engine();
	// put the destructor in the cpp file since
	// it needs the definition of the Box, Particles, Integrator, etc
	~Engine();
	void resetBox(unique_ptr<Box>& box_);
	void resetParticles(unique_ptr<Particles> particles_);
	void registerBox(unique_ptr<Box>& box_);
	void registerParticles(unique_ptr<Particles>& particles_);
	void registerIntegrator(unique_ptr<Integrator>& integrator_);
	void registerFix(unique_ptr<Fix> fix_);
	const unique_ptr<Box>& getBox() const;
	const unique_ptr<Particles>& getParticles() const;
	unique_ptr<Particles>& getParticlesForUpdate();
	unique_ptr<Integrator>& getIntegrator();
	vector<unique_ptr<Fix>>& getFixList();
	const unique_ptr<Fix>& returnFixById(string id_) const;

private:
	unique_ptr<Box> box = nullptr;
	unique_ptr<Particles> particles = nullptr;
	unique_ptr<Integrator> integrator = nullptr;
	vector<unique_ptr<Fix>> fixList;
	int nmax;
};
