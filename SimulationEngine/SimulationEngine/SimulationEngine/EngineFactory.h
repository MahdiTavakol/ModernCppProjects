#pragma once
#include "Engine.h"

#include <vector>
#include <string>

constexpr int maxCommandTypes = 20;

class EngineFactory {
public:
	EngineFactory(std::vector<std::string> args);
	std::unique_ptr<Engine> returnEngine();

	// Classes with multiple inheritance levels
	// they finally will have their own factory class
	void buildIntegrator(std::vector<std::string> args_);
	void buildFix(std::vector<std::string> args_);
	void buildForceField(std::vector<std::string> args_);
	void buildNeighbor(std::vector<std::string> args_);

private:
	std::vector<std::string> args;
	void parseCommand(const std::string& command);
	void commandNumCheck(int& itemCount, const std::string commandType);
	void addParticle(std::vector<std::string> args_);

	// Tracking the number of each commands
	int commandCount[maxCommandTypes];

	// since the items need a refence to the engine
	// first a default engine needs to be created
	// and then items can be created with reference to the engine and stored in the factory
	std::unique_ptr<Engine> engine;
	// the items created by the factory to be added to the engine
	std::unique_ptr<Box> box;
	std::unique_ptr<Error> error;
	std::vector<std::unique_ptr<Fix>> fixList;
	std::unique_ptr<FixList> fixListObj;
	std::unique_ptr<ForceField> forcefield;
	std::unique_ptr<Integrator> integrator;
	std::unique_ptr<Neighbor> neighbor;
	std::unique_ptr<Particles> particles;
	std::unique_ptr<Run> run;
};
