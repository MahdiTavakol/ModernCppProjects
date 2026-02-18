#pragma once

#include <memory>
#include <vector>
#include <string>

class Box;
class Particles;
class Integrator;
class ForceField;
class Fix;


class Engine {
public:
	enum class ItemType { BOX, PARTICLES };

	// constructor with prebuilt types
	Engine(std::unique_ptr<Box>& box_,
		std::unique_ptr<Particles>& particles_);
	// default constructor to be used in unittests
	Engine();
	// put the destructor in the cpp file since
	// it needs the definition of the Box, Particles, Integrator, etc
	~Engine();
	void resetBox(std::unique_ptr<Box>& box_);
	void resetParticles(std::unique_ptr<Particles> particles_);
	void registerBox(std::unique_ptr<Box>& box_);
	void registerParticles(std::unique_ptr<Particles>& particles_);
	void registerIntegrator(std::unique_ptr<Integrator>& integrator_);
	void registerForceField(std::unique_ptr<ForceField>& forcefield_);
	void registerFix(std::unique_ptr<Fix> fix_);
	const std::unique_ptr<Box>& getBox() const;
	const std::unique_ptr<Particles>& getParticles() const;
	std::unique_ptr<Particles>& getParticlesForUpdate();
	std::unique_ptr<Integrator>& getIntegrator();
	std::vector<std::unique_ptr<Fix>>& getFixList();
	std::unique_ptr<Fix>& returnFixById(std::string id_);

private:
	std::unique_ptr<Box> box = nullptr;
	std::unique_ptr<Particles> particles = nullptr;
	std::unique_ptr<Integrator> integrator = nullptr;
	std::unique_ptr<ForceField> forcefield = nullptr;
	std::vector<std::unique_ptr<Fix>> fixList;
	int nmax;
};
