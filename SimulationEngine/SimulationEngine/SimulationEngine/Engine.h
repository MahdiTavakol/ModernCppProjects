#pragma once

#include <memory>
#include <vector>
#include <string>

class Box;
class Particles;
class Integrator;
class ForceField;
class FixList;
class Error;
class Fix;
class Neighbor;
class Ref;
class Run;


class Engine {
public:
	enum class Run_Status {SILENT,VERBOSE};
	enum class ItemRef {  UNKNOWN,
		                  BOX,
		                  PARTICLES,
	                      INTEGRATOR,
	                      FORCEFIELD,
	                      FIX,
	                      NEIGHBOR,
	                      ERROR,
	                      RUN};

	// constructor with prebuilt Refs
	Engine(std::unique_ptr<Box>& box_,
		std::unique_ptr<Particles>& particles_);
	// default constructor to be used in unittests
	Engine();
	// Since the engine is a resource manager we need to remove the copy constructor and copy assignment operator
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	// here we just have unique_ptr and vector which are movable so we can
	// just use the default move constructor and move assignment operator
	Engine(Engine&&) noexcept = default;
	Engine& operator=(Engine&&) noexcept = default;
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
	std::unique_ptr<FixList>& getFixList();
	std::unique_ptr<Fix>& returnFixById(std::string id_);
	std::unique_ptr<Neighbor>& getNeighbor();
	std::unique_ptr<Error>& getError();
	// getting the run status of the engine
	void setStatus(const std::string newStatus_);
	const Run_Status& getStatus() const;
	std::unique_ptr<Run>& getRunCommand();

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
	std::unique_ptr<FixList> fixList;
	std::unique_ptr<Error> error;
	std::unique_ptr<Run> run;

	int nmax;

};
