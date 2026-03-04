#include "Communicator.h"
#include <algorithm>
#include <iostream>
#include <utility>

Communicator::Communicator() :
	Ref{"1"},
	skin{ 0.0 },
	myId{ 0 },
	sizeArray{1,1,1}
{}

Communicator::Communicator(std::vector<std::string>& args_) :
	Ref{ "1" },
	skin{ 0.0 }
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

Communicator::Communicator(const int& myId_, const int& size_, const double& skin_) :
	Ref{ "1" },
	skin{skin_},
	myId{myId_},
	sizeArray{size_,1,1}
{}

Communicator::Communicator(const int& myId_, const std::array<int, 3>& sizeArray_, const double& skin_):
	Ref{"1"},
	skin{skin_},
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
	int originalNlocal = 0;
	particles->getNmaxNlocal(nmax, originalNlocal);
	std::unique_ptr<Particles> newParticles = std::make_unique<Particles>(nmax);

	std::vector<int> ghostIds;
	nlocal = 0;
	nghosts = 0;


	double xMinGhost = myMin[0] - skin;
	double xMaxGhost = myMax[0] + skin;

	double yMinGhost = myMin[1] - skin;
	double yMaxGhost = myMax[1] + skin;

	double zMinGhost = myMin[2] - skin;
	double zMaxGhost = myMax[2] + skin;

 
	for (int i = 0; i < originalNlocal; i++) {
		double x = particles->X(i, 0);
		double y = particles->X(i, 1);
		double z = particles->X(i, 2);
		bool insideOwned =
			(x >= myMin[0] && x < myMax[0] &&
			 y >= myMin[1] && y < myMax[1] &&
			 z >= myMin[2] && z < myMax[2]);

		bool insideExtended1 =
			(x >= xMinGhost && x < xMaxGhost &&
			 y >= myMin[1]  && y < myMax[1]  &&
			 z >= myMin[2]  && z < myMax[2]);

		bool insideExtended2 =
			(x >= myMin[0]  && x < myMax[0]  &&
			 y >= yMinGhost && y < yMaxGhost &&
			 z >= myMin[2]  && z < myMax[2]);

		bool insideExtended3 =
			(x >= myMin[0]  && x < myMax[0] &&
			 y >= myMin[1]  && y < myMax[1] &&
			 z >= zMinGhost && z < zMinGhost);



  		if (insideOwned) {
			nlocal++;
			newParticles->copyParticle(particles, i);
		}
		

		if (!insideOwned && (insideExtended1 ||
			                 insideExtended2 ||
			                 insideExtended3 ))
		{
			nghosts++;
			ghostIds.push_back(i);
		}


	}


	for (const auto& id : ghostIds) {
		newParticles->copyParticle(particles, id);
	}

	newParticles->setNGhosts(nghosts);
	newParticles->setNmaxNlocal(nmax, nlocal);

	*particles = std::move(*newParticles);
}