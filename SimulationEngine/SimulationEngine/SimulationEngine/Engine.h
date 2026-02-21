#pragma once

#include <memory>
#include <vector>
#include <string>

class Box;
class Particles;
class Integrator;
class ForceField;
class Error;
class Fix;
class Neighbor;
class Ref;


class Engine {
public:
	enum class Run_Status {SILENT,VERBOSE};
	enum class ItemRef { UNKNOWN,
		                  BOX,
		                  PARTICLES,
	                      INTEGRATOR,
	                      FORCEFIELD,
	                      FIX,
	                      NEIGHBOR,
	                      ERROR};

	// constructor with prebuilt Refs
	Engine(std::unique_ptr<Box>& box_,
		std::unique_ptr<Particles>& particles_);
	// default constructor to be used in unittests
	Engine();
	// Since the engine is a resource manager we need to remove the copy constructor and copy assignment operator
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	// we do not need the move constructor and move assignment operator for now, but we can add them later if needed
	Engine(Engine&&) = delete;
	Engine& operator=(Engine&&) = delete;
	// passing the run_status as an argument
	Engine(Run_Status& run_status_);
	// put the destructor in the cpp file since
	// it needs the definition of the Box, Particles, Integrator, etc
	~Engine();
	// setting items into the engine, the Ref of the item will be detected by the engine and put into the right place
	void setItem(std::unique_ptr<Ref>&& Ref_);
	// getting the item by the Ref, the return Ref is Ref* and needs to be casted to the right Ref by the user
	Ref* getItem(ItemRef Ref_);
	// getting the item by the Ref, the return Ref is the unique_ptr of the right Ref
	const std::unique_ptr<Box>& getBox() const;
	const std::unique_ptr<Particles>& getParticles() const;
	std::unique_ptr<Particles>& getParticlesForUpdate();
	std::unique_ptr<Integrator>& getIntegrator();
	std::unique_ptr<ForceField>& getForceField();
	std::vector<std::unique_ptr<Fix>>& getFixList();
	std::unique_ptr<Fix>& returnFixById(std::string id_);
	std::unique_ptr<Neighbor>& getNeighbor();
	std::unique_ptr<Error>& getError();
	// getting the run status of the engine
	const Run_Status& getStatus() const;

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
	std::unique_ptr<Error> error;
	int nmax;

	// a helper function to detect the Ref of the input unique_ptr<Ref> 
	static ItemRef detectRef(const std::unique_ptr<Ref>& Ref_);
};
