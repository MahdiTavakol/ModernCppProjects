#include "Communicator.h"
#include <algorithm>

Communicator::Communicator(const int& myId_, const int& size_) :
	Ref{ "1" },
	myId{myId_},
	size{size_}
{}

void Communicator::injectDependencies(Engine& engine_)
{
	particles = engine_.getParticlesForUpdate().get();
	box = engine_.getBox().get();
	engine = &engine_;
}

void Communicator::init()
{
	resetParticles();
	box->getDimensions(myMin, myMax);
	double xRange, yRange, zRange;
	xRange = myMax[0] - myMin[0];
	yRange = myMax[1] - myMin[1];
	zRange = myMax[2] - myMin[2];
	double xRangePerNode = xRange / static_cast<double>(sizeX);
	double yRangePerNode = yRange / static_cast<double>(sizeY);
	double zRangePerNode = zRange / static_cast<double>(sizeZ);
	int myIdX = myId / sizeX;
	int myIdY = myId / sizeY;
	int myIdZ = myId / sizeZ;
	myMin[0] += myIdX * xRangePerNode;
	myMin[1] += myIdY * yRangePerNode;
	myMin[2] += myIdZ * zRangePerNode;
	myMax[0] = myMin[0] + xRangePerNode;
	myMax[1] = myMin[1] + yRangePerNode;
	myMax[2] = myMin[2] + zRangePerNode;
	// setting the min and max size of the box
	box->setDimensions(myMin, myMax);
}

void Communicator::resetParticles()
{
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
	engine->resetParticles(std::move(newParticles));

}