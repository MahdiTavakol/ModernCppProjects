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
	myId{myId_}
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
	// setting the forward and reverse partners
	// xlo partner
	forward_partner[0][0] = myIdX - 1;
	// xhi partner
	forward_partner[0][1] = myIdX + 1;
	// ylo partner
	forward_partner[1][0] = myIdY - sizeArray[1];
	// yhi partner
	forward_partner[1][1] = myIdY + sizeArray[1];
	// zlo partner
	forward_partner[2][0] = myIdZ + sizeArray[0] * sizeArray[1];
	// zhi partner
	forward_partner[2][1] = myIdZ + sizeArray[0] * sizeArray[1];
	// xlo partner 
	reverse_partner[0][0] = myIdX - 1;
	// xhi partner
	reverse_partner[0][1] = myIdX + 1;
	// ylo partner
	reverse_partner[1][0] = myIdY - sizeArray[1];
	// yhi partner
	reverse_partner[1][1] = myIdY + sizeArray[1];
	// zlo partner
	reverse_partner[2][0] = myIdZ + sizeArray[0] * sizeArray[1];
	// zhi partner
	reverse_partner[2][1] = myIdZ + sizeArray[0] * sizeArray[1];
}

void Communicator::forward_particle(
	const std::array<int, 3>& dir_,
	transferedData& trandata_)
{
	std::vector<int>* vecRef = &interXLo;
	
	if (dir_ == std::array<int, 3>{-1, 0, 0}) {
		vecRef = &interXLo;
	}
	else if (dir_ == std::array<int, 3>{1, 0, 0}) {
		vecRef = &interXHi;
	}
	else if (dir_ == std::array<int, 3>{0, -1, 0}) {
		vecRef = &interYLo;
	}
	else if (dir_ == std::array<int, 3>{0, 1, 0}) {
		vecRef = &interYHi;
	}
	else if (dir_ == std::array<int, 3>{0, 0, -1}) {
		vecRef = &interZLo;
	}
	else if (dir_ == std::array<int, 3>{0, 0, 1}) {
		vecRef = &interZHi;
	}
	else
		throw std::invalid_argument("Wrong direction!");



	for (const auto& id : *vecRef)
	{
		double x0 = particles->X(id, 0);
		double x1 = particles->X(id, 1);
		double x2 = particles->X(id, 2);
		double v0 = particles->V(id, 0);
		double v1 = particles->V(id, 1);
		double v2 = particles->V(id, 2);
		double f0 = particles->F(id, 0);
		double f1 = particles->F(id, 1);
		double f2 = particles->F(id, 2);
		double m  = particles->M(id);
		double r  = particles->R(id);

		trandata_.xData.push_back({ x0,x1,x2 });
		trandata_.vData.push_back({ v0,v1,v2 });
		trandata_.fData.push_back({ f0,f1,f2 });
		trandata_.mData.push_back(m);
		trandata_.rData.push_back(r);
	}
}

void Communicator::updateGhosts(const std::array<int, 3>& dir_, const transferedData& trandata_)
{
	std::vector<int>* vecRef;

	if (dir_ == std::array<int, 3>{-1, 0, 0}) {
		vecRef = &ghostsXLo;
	}
	else if (dir_ == std::array<int, 3>{1, 0, 0}) {
		vecRef = &ghostsXHi;
	}
	else if (dir_ == std::array<int, 3>{0, -1, 0}) {
		vecRef = &ghostsYLo;
	}
	else if (dir_ == std::array<int, 3>{0, 1, 0}) {
		vecRef = &ghostsYHi;
	}
	else if (dir_ == std::array<int, 3>{0, 0, -1}) {
		vecRef = &ghostsZLo;
	}
	else if (dir_ == std::array<int, 3>{0, 0, 1}) {
		vecRef = &ghostsZHi;
	}
	else
		throw std::invalid_argument("Wrong direction!");

	// checking the transfered data size
	if (trandata_.xData.size() != (*vecRef).size() ||
		trandata_.vData.size() != (*vecRef).size() ||
		trandata_.fData.size() != (*vecRef).size() ||
		trandata_.mData.size() != (*vecRef).size() ||
		trandata_.rData.size() != (*vecRef).size()) {
		throw std::invalid_argument("Wrong data size");
	}

	auto dataSize = trandata_.xData.size();


	for (int i = 0; i < dataSize; i++) {
		int id = (*vecRef)[i];
		particles->X(id, 0) = trandata_.xData[i][0];
		particles->X(id, 1) = trandata_.xData[i][1];
		particles->X(id, 2) = trandata_.xData[i][2];
		particles->V(id, 0) = trandata_.vData[i][0];
		particles->V(id, 1) = trandata_.vData[i][1];
		particles->V(id, 2) = trandata_.vData[i][2];
		particles->F(id, 0) = trandata_.fData[i][0];
		particles->F(id, 1) = trandata_.fData[i][1];
		particles->F(id, 2) = trandata_.fData[i][2];
		particles->M(id) = trandata_.mData[i];
		particles->R(id) = trandata_.rData[i];
	}
}

void Communicator::resetParticles()
{
	int originalNlocal = 0;
	particles->getNmaxNlocal(nmax, originalNlocal);
	std::unique_ptr<Particles> newParticles = std::make_unique<Particles>(nmax);

	nlocal = 0;
	nghosts = 0;


	double xMinGhost = myMin[0] - skin;
	double xMaxGhost = myMax[0] + skin;

	double yMinGhost = myMin[1] - skin;
	double yMaxGhost = myMax[1] + skin;

	double zMinGhost = myMin[2] - skin;
	double zMaxGhost = myMax[2] + skin;

	double xMinInter = myMin[0] + skin;
	double xMaxInter = myMax[0] - skin;

	double yMinInter = myMin[1] + skin;
	double yMaxInter = myMax[1] - skin;

	double zMinInter = myMin[2] + skin;
	double zMaxInter = myMax[2] - skin;


	for (int i = 0; i < originalNlocal; i++) {
		double x = particles->X(i, 0);
		double y = particles->X(i, 1);
		double z = particles->X(i, 2);
		bool insideOwned =
			(x >= myMin[0] && x < myMax[0] &&
				y >= myMin[1] && y < myMax[1] &&
				z >= myMin[2] && z < myMax[2]);

		// its own ghosts
		bool insideExtended1 =
			(x >= xMinGhost && x < xMaxGhost &&
				y >= myMin[1] && y < myMax[1] &&
				z >= myMin[2] && z < myMax[2]);

		bool insideExtended2 =
			(x >= myMin[0] && x < myMax[0] &&
				y >= yMinGhost && y < yMaxGhost &&
				z >= myMin[2] && z < myMax[2]);

		bool insideExtended3 =
			(x >= myMin[0] && x < myMax[0] &&
				y >= myMin[1] && y < myMax[1] &&
				z >= zMinGhost && z < zMinGhost);

		// its own atoms acting as ghosts for neighboring ranks
		bool insideInter1 =
			(x >= xMinInter && x < xMaxInter &&
				y >= myMin[1] && y < myMax[1] &&
				z >= myMin[2] && z < myMax[2]);

		bool insideInter2 =
			(x >= myMin[0] && x < myMax[0] &&
				y >= yMinInter && y < yMaxInter &&
				z >= myMin[2] && z < myMax[2]);

		bool insideInter3 =
			(x >= myMin[0] && x < myMax[0] &&
				y >= myMin[1] && y < myMax[1] &&
				z >= zMinInter && z < zMaxInter);


		// inside the region 
		if (insideOwned) {
			nlocal++;
			newParticles->copyParticle(particles, i);
			// checking the interior atoms
			if (!insideInter1 && x <= xMinInter)
				interXLo.push_back(nlocal - 1);
			else if (!insideInter1 && x > xMaxInter)
				interXHi.push_back(nlocal - 1);
			if (!insideInter2 && y <= yMinInter)
				interYLo.push_back(nlocal - 1);
			else if (!insideInter2 && y > yMaxInter)
				interYHi.push_back(nlocal - 1);
			if (!insideInter3 && z <= zMinInter)
				interZLo.push_back(nlocal - 1);
			else if (!insideInter3 && z > zMaxInter)
				interZHi.push_back(nlocal - 1);
		}
		// in the ghost region
		else if (insideExtended1 || insideExtended2 || insideExtended3) {
			ghostIds.push_back(i);
			if (insideExtended1 && x < myMin[0]) {
				ghostsXLo.push_back(nghosts++);
			}
			else if (insideExtended1 && x > myMax[0]) {
				ghostsXHi.push_back(nghosts++);
			}
			else if (insideExtended2 && y < myMin[1]) {
				ghostsYLo.push_back(nghosts++);
			}
			else if (insideExtended2 && y > myMax[1]) {
				ghostsYHi.push_back(nghosts++);
			}
			else if (insideExtended3 && z < myMin[2]) {
				ghostsZLo.push_back(nghosts++);
			}
			else if (insideExtended3 && z > myMax[2]) {
				ghostsZHi.push_back(nghosts++);
			}
		}
		// outside ghost region
		else
		{
			// do nothing!
		}

	}


	auto offset = [&](int a) {return a + nlocal; };
	std::transform(ghostsXLo.begin(), ghostsXLo.end(), ghostsXLo.begin(), offset);
	std::transform(ghostsXHi.begin(), ghostsXHi.end(), ghostsXHi.begin(), offset);
	std::transform(ghostsYLo.begin(), ghostsYLo.end(), ghostsYLo.begin(), offset);
	std::transform(ghostsYHi.begin(), ghostsYHi.end(), ghostsYHi.begin(), offset);
	std::transform(ghostsZLo.begin(), ghostsZLo.end(), ghostsZLo.begin(), offset);
	std::transform(ghostsZHi.begin(), ghostsZHi.end(), ghostsZHi.begin(), offset);


	for (const auto& id : ghostIds) {
		newParticles->copyParticle(particles, id);
	}


	newParticles->setNGhosts(nghosts);
	newParticles->setNmaxNlocal(nmax, nlocal);
	*particles = std::move(*newParticles);
}