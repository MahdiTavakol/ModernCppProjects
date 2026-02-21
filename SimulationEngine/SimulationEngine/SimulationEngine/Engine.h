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
	enum class ItemType { UNKNOWN,
		                  BOX,
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
	// setting items into the engine, the type of the item will be detected by the engine and put into the right place
	void setItem(std::unique_ptr<Type>&& type_);
	// getting the item by the type, the return type is Type* and needs to be casted to the right type by the user
	Type* getItem(ItemType type_);
	// getting the item by the type, the return type is the unique_ptr of the right type
	const std::unique_ptr<Box>& getBox() const;
	const std::unique_ptr<Particles>& getParticles() const;
	std::unique_ptr<Particles>& getParticlesForUpdate();
	std::unique_ptr<Integrator>& getIntegrator();
	std::unique_ptr<ForceField>& getForceField();
	std::vector<std::unique_ptr<Fix>>& getFixList();
	std::unique_ptr<Fix>& returnFixById(std::string id_);
	std::unique_ptr<Neighbor>& getNeighbor();
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
	int nmax;

	// a helper function to detect the type of the input unique_ptr<Type> 
	static ItemType detectType(const std::unique_ptr<Type>& type_);
};
