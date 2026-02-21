
#include "EngineFactory.h"

#include "Box.h"
#include "Error.h"
#include "Fix.h"
#include "ForceField.h"
#include "Integrator.h"
#include "Neighbor.h"
#include "Particles.h"
#include "Run.h"


// Integrator class
#include "EulerIntegrator.h"
#include "SemiIntegrator.h"

// Fix types
#include "FixPrint.h"



#include <sstream>
#include <stdexcept>


using std::vector;
using std::string;
using std::unique_ptr;
using std::stringstream;

EngineFactory::EngineFactory(std::vector<std::string> args) : 
	args(std::move(args))

{
	// reseting the number of each command type
	std::fill_n(commandCount, maxCommandTypes, 0);
	// constructing the default engine to be used for creating items
	engine = std::make_unique<Engine>();
	// constructing items with default constructor
	// each one is constructed with just reference 
	// so that each class itself can handle the default values 
	// of its parameters and the error handling of missing parameters
	// to avoid coupling the factory with the details of each class.
	box = std::make_unique<Box>(*engine);
	error = std::make_unique<Error>(*engine);
	forcefield = std::make_unique<ForceField>(*engine);
	integrator = std::make_unique<Integrator>(*engine);
	neighbor = std::make_unique<Neighbor>(*engine);
	particles = std::make_unique<Particles>(*engine);
	run = std::make_unique<Run>(*engine);
}

Engine&& EngineFactory::returnEngine() {
	for (const auto& command : args) {
		this->parseCommand(command);
	}
	if (engine == nullptr) {
		throw std::runtime_error("The engine is not created successfully");
	}
	// adding the items created by the factory into the engine
	engine->setItem(std::move(box));
	engine->setItem(std::move(error));
	engine->setItem(std::move(forcefield));
	engine->setItem(std::move(integrator));
	engine->setItem(std::move(neighbor));
	engine->setItem(std::move(particles));
	engine->setItem(std::move(run));
	engine->getFixList() = std::move(fixList);

	return std::move(*engine);
}

void EngineFactory::parseCommand(const std::string& command) {
	std::vector<std::string> tokens;
	stringstream ss(command);
	string istring;
	while (ss >> istring) {
		tokens.push_back(istring);
	}

	int itemId = 0;
	if (++itemId && tokens[0] == "box") {
		// parse box command
		commandNumCheck(commandCount[itemId-1], tokens[0]);
		box = make_unique<Box>(*engine, tokens);
	} else if (++itemId && tokens[0] == "error") {
		// parse particles command
		commandNumCheck(commandCount[itemId-1], tokens[0]);
		error = make_unique<Error>(*engine, tokens);
	} else if (++itemId && tokens[0] == "fix") {
		// parse fix command
		buildFix(tokens);
	} else if (++itemId && tokens[0] == "forcefield") {
		// parse forcefield command
		commandNumCheck(commandCount[itemId-1], tokens[0]);
		//forcefield = make_unique<ForceField>(*engine, tokens);
	} else if (++itemId && tokens[0] == "integrator") {
		// parse integrator command
		commandNumCheck(commandCount[itemId-1], tokens[0]);
		buildIntegrator(tokens);
	} else if (++itemId && tokens[0] == "neighbor") {
		// parse neighbor command
		commandNumCheck(commandCount[itemId-1], tokens[0]);
		//neighbor = make_unique<Neighbor>(*engine, tokens);
	} else if (++itemId && tokens[0] == "particles") {
		// parse particles command
		commandNumCheck(commandCount[itemId-1], tokens[0]);
		particles = make_unique<Particles>(*engine, tokens);
	} else if (++itemId && tokens[0] == "run") {
		// parse run command
		commandNumCheck(commandCount[itemId-1], tokens[0]);
		run = make_unique<Run>(*engine, tokens);
	}
	else if (++itemId && tokens[0] == "run_status") {
		commandNumCheck(commandCount[itemId - 1], tokens[0]);
		engine->setStatus(tokens[1]);
	}
	else {
		(*error) << "Unknown command: " << tokens[0] << std::endl;
	}

}

void EngineFactory::commandNumCheck(int& itemCount, const std::string commandType) {
	itemCount++;
	if (itemCount > 1) {
		(*error) << "Duplicate command: " << commandType << " " << itemCount << std::endl;
		(*error) << "Using the last one" << std::endl;
		return;
	}
}

void EngineFactory::buildIntegrator(std::vector<std::string> args_) {
	// parse the integrator command and create the integrator
	if (args_.size() < 2) {
		(*error) << "integrator command needs a type (euler/semi)\n";
	}
	if (args_[1] == "euler")
		integrator = make_unique<EulerIntegrator>(*engine, args_);
	else if (args_[1] == "semi")
		integrator = make_unique<SemiIntegrator>(*engine, args_);
	else
		(*error) << "Unknown integrator type: " << args_[1] << std::endl;
}

void EngineFactory::buildFix(std::vector<std::string> args_) {
	if (args_.size() < 2) {
		(*error) << "fix command needs a type\n";
	}
	if (args_[1] == "print") {
		fixList.push_back(make_unique<FixPrint>(*engine, args_));
	}
	else {
		(*error) << "Unknown fix type: " << args_[1] << std::endl;
	}
}