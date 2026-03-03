#include "Communicator.h"
#include <algorithm>
#include <iostream>

Communicator::Communicator() :
	Ref{"1"},
	myId{ 0 },
	sizeArray{1,1,1}
{}

Communicator::Communicator(std::vector<std::string>& args_) :
	Ref{ "1" }
{
	// communicator sizeX [sizeY sizeZ]
	int nargs = args_.size();
	int sizeX = 1;
	int sizeY = 1;
	int sizeZ = 1;
	if (nargs == 4) {
		sizeX = std::stoi(args_[1]);
		sizeY = std::stoi(args_[2]);
		sizeZ = std::stoi(args_[3]);
	}
	else if (nargs == 2) {
		int sizeX = std::stoi(args_[1]);
	}
	else
		throw std::invalid_argument("Wrong input arguments!");
	
	sizeArray = std::array<int, 3>{ sizeX,sizeY,sizeZ };
}

Communicator::Communicator(const int& myId_, const int& size_) :
	Ref{ "1" },
	myId{myId_},
	sizeArray{size_,1,1}
{}

Communicator::Communicator(const int& myId_, const std::array<int, 3>& sizeArray_):
	Ref{"1"},
	myId{myId_},
	sizeArray{sizeArray_}
{}

void Communicator::injectDependencies(Engine& engine_)
{
	particles = engine_.getParticlesForUpdate().get();
	box = engine_.getBox().get();
	engine = &engine_;
}

void Communicator::init()
{
	box->getDimensions(myMin, myMax);
 	double xRange, yRange, zRange;
	xRange = myMax[0] - myMin[0];
	yRange = myMax[1] - myMin[1];
	zRange = myMax[2] - myMin[2];
	double xRangePerNode = xRange / static_cast<double>(sizeArray[0]);
	double yRangePerNode = yRange / static_cast<double>(sizeArray[1]);
	double zRangePerNode = zRange / static_cast<double>(sizeArray[2]);
	int myIdX = myId % sizeArray[0];
	int myIdY = myId / sizeArray[1];
	int myIdZ = myId / (sizeArray[0]*sizeArray[1]);
	myMin[0] += myIdX * xRangePerNode;
	myMin[1] += myIdY * yRangePerNode;
	myMin[2] += myIdZ * zRangePerNode;
	myMax[0] = myMin[0] + xRangePerNode;
	myMax[1] = myMin[1] + yRangePerNode;
	myMax[2] = myMin[2] + zRangePerNode;
	// setting the min and max size of the box
	box->setDimensions(myMin, myMax);
	// resetting the particles based on the new dimensions
	this->resetParticles();
}

void Communicator::resetParticles()
{
	particles->getNmaxNlocal(nmax, nlocal);
	std::unique_ptr<Particles> newParticles = std::make_unique<Particles>();
 
	for (int i = 0; i < nlocal; i++) {
		double x = particles->X(i, 0);
		double y = particles->X(i, 1);
		double z = particles->X(i, 2);
  		if (x >= myMin[0] &&
			x < myMax[0] &&
			y >= myMin[1] &&
			y < myMax[1] &&
			z >= myMin[2] &&
			z < myMax[2]) {
			newParticles->copyParticle(particles, i);
		}
	}
	newParticles->setNmax(nmax);
	engine->resetParticles(std::move(newParticles));
}