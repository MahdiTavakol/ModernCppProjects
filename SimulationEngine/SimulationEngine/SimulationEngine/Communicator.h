#pragma once
#include "Engine.h"
#include "Ref.h"
#include "Particles.h"
#include "Box.h"

class Communicator : public Ref {
public:
	Communicator();
	Communicator(std::vector<std::string>& args_);
	Communicator(const int& myId_, const int& size_, const double& skin_ = 0.0);
	Communicator(const int& myId_, const std::array<int, 3>& size_, const double& skin_ = 0.0);
	void injectDependencies(Engine& engine_) override;
	void init();

	
protected:
	// box ref
	Box* box = nullptr;
	// Particles ref
	Particles* particles = nullptr;
	// we need a pointer to the engine
	Engine* engine = nullptr;
	// resetting the particles
	void resetParticles();
	// the box dimensions
	std::array<double, 3> myMin = { 0,0,0 }, myMax = { 0,0,0 };

	// number of particles
	int nlocal = 0, nmax = 0, nghosts = 0;
	// skin
	double skin = 0.0;
	// communicator id
	int myId = 0;
	// communicator configuration
	std::array<int, 3> sizeArray = { 2,2,1 };
};
