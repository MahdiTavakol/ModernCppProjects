#pragma once

#include <memory>
#include <vector>
#include <string>

class Box;
class Particles;
class Integrator;
class ForceField;
class Fix;
class Neighbor;
class Type;


class Engine {
public:
	enum class Run_Status {SILENT,VERBOSE};
	enum class ItemType { BOX,
		                  PARTICLES,
	                      INTEGRATOR,
	                      FORCEFIELD,
	                      FIX,
	                      NEIGHBOR};

	// constructor with prebuilt types
	Engine(std::unique_ptr<Box>& box_,
		std::unique_ptr<Particles>& particles_);
	// default constructor to be used in unittests
	Engine();
	// passing the run_status as an argument
	Engine(Run_Status& run_status_);
	// put the destructor in the cpp file since
	// it needs the definition of the Box, Particles, Integrator, etc
	~Engine();
	void registerItem(std::unique_ptr<Type>&& type_);
	void resetBox(std::unique_ptr<Box>& box_);
	void resetParticles(std::unique_ptr<Particles> particles_);
	void registerBox(std::unique_ptr<Box>& box_);
	void registerParticles(std::unique_ptr<Particles>& particles_);
	void registerIntegrator(std::unique_ptr<Integrator>& integrator_);
	void registerForceField(std::unique_ptr<ForceField>& forcefield_);
	void registerFix(std::unique_ptr<Fix> fix_);
	void registerNeighbor(std::unique_ptr<Neighbor>& neighbor_);
	const std::unique_ptr<Box>& getBox() const;
	const std::unique_ptr<Particles>& getParticles() const;
	std::unique_ptr<Particles>& getParticlesForUpdate();
	std::unique_ptr<Integrator>& getIntegrator();
	std::unique_ptr<ForceField>& getForceField();
	std::vector<std::unique_ptr<Fix>>& getFixList();
	std::unique_ptr<Fix>& returnFixById(std::string id_);
	const Run_Status& getStatus() const;
	std::unique_ptr<Neighbor>& getNeighbor();
	std::unique_ptr<Type>& getItem(ItemType type_);

private:
	Run_Status run_status = Run_Status::SILENT;
	std::unique_ptr<Box> box = nullptr;
	std::unique_ptr<Particles> particles = nullptr;
	std::unique_ptr<Integrator> integrator = nullptr;
	/* 
	 * The default value of the forcefield 
	 * is provided in the constructor
	 */
	std::unique_ptr<ForceField> forcefield;
	std::unique_ptr<Neighbor> neighbor;
	std::vector<std::unique_ptr<Fix>> fixList;
	int nmax;
};
