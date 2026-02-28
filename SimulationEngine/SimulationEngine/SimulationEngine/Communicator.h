#pragma once
#include "Engine.h"
#include "Ref.h"
#include "Particles.h"
#include "Box.h"

class Communicator : public Ref {
public:
	Communicator(const int& myId_, const int& size_);
	void injectDependencies(Engine& engine_) override;
	void init();
	
protected:
	// box ref
	Box* box;
	// Particles ref
	Particles* particles;
	// we need a pointer to the engine
	Engine* engine;
	void resetParticles();

	std::array<double, 3> myMin, myMax;


	int nlocal, nmax;
	int myId;
	int size;
	int sizeX = 2, sizeY = 2, sizeZ = 1;
};
