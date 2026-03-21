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
		sizeX = std::stoi(args_[1]);
	}
	else
		throw std::invalid_argument("Wrong input arguments!");
	
	sizeArray = std::array<int, 3>{ sizeX,sizeY,sizeZ };
}

Communicator::Communicator(const int& myId_, const int& size_, const double& skin_) :
	Ref{ std::to_string(myId_)},
	skin{skin_},
	myId{myId_}
{}

Communicator::Communicator(const int& myId_, const std::array<int, 3>& sizeArray_, const double& skin_):
	Ref{std::to_string(myId_)},
	skin{skin_},
	myId{myId_},
	sizeArray{sizeArray_}
{}

Communicator::~Communicator()
{

}

void Communicator::injectDependencies(Engine& engine_)
{
	particles = engine_.getParticlesForUpdate().get();
	box = engine_.getBox().get();
	engine = &engine_;
}

void Communicator::init()
{
	// resetting the exchangeMessages
	std::for_each(exchangeMessages, exchangeMessages + 6,
		[&](std::vector<double>& a) {
			/*
			* format 
			* nParticles
			* x[0]
			* x[1]
			* x[2]
			* v[0]
			* v[1]
			* v[2]
			* f[0]
			* f[1]
			* f[2]
			* r
			* m
			* so here we just have one data
			* which is the 0!
			*/
			a.push_back(0.0);
		});
	//
	box->getDimensions(myMin, myMax);
 	double xRange, yRange, zRange;
	xRange = myMax[0] - myMin[0];
	yRange = myMax[1] - myMin[1];
	zRange = myMax[2] - myMin[2];
	double xRangePerNode = xRange / static_cast<double>(sizeArray[0]);
	double yRangePerNode = yRange / static_cast<double>(sizeArray[1]);
	double zRangePerNode = zRange / static_cast<double>(sizeArray[2]);
	int myIdX = myId % sizeArray[0];
	int myIdY = (myId / sizeArray[0]) % sizeArray[1];
	int myIdZ = (myId / (sizeArray[0]*sizeArray[1])) % sizeArray[2];
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
	int xlo = myId - 1;
	forward_partner[0][0] = myIdX > 0 ? xlo : -1;
	// xhi partner
	int xhi = myId + 1;
	forward_partner[0][1] = myIdX < sizeArray[0]-1 ? xhi : -1;
	// ylo partner
	int ylo = myId - sizeArray[0];
	forward_partner[1][0] = myIdY > 0 ? ylo : -1;
	// yhi partner
	int yhi = myId + sizeArray[0];
	forward_partner[1][1] = myIdY < sizeArray[1]-1 ? yhi : -1;
	// zlo partner
	int zlo = myId - sizeArray[0] * sizeArray[1];
	forward_partner[2][0] = myIdZ > 0 ? zlo : -1;
	// zhi partner
	int zhi = myId + sizeArray[0] * sizeArray[1];
	forward_partner[2][1] = myIdZ < sizeArray[2]-1 ? zhi : -1;
	// xlo partner 
	reverse_partner[0][0] = forward_partner[0][0];
	// xhi partner
	reverse_partner[0][1] = forward_partner[0][1];
	// ylo partner
	reverse_partner[1][0] = forward_partner[1][0];
	// yhi partner
	reverse_partner[1][1] = forward_partner[1][1];
	// zlo partner
	reverse_partner[2][0] = forward_partner[2][0];
	// zhi partner
	reverse_partner[2][1] = forward_partner[2][1];
	// xlo partner
	neighbor_ranks[0][0] = forward_partner[0][0];
	// xhi partner
	neighbor_ranks[0][1] = forward_partner[0][1];
	// ylo partner
	neighbor_ranks[1][0] = forward_partner[1][0];
	// yhi partner
	neighbor_ranks[1][1] = forward_partner[1][1];
	// zlo partner 
	neighbor_ranks[2][0] = forward_partner[2][0];
	// zhi partner
	neighbor_ranks[2][1] = forward_partner[2][1];

}

std::vector<int>* Communicator::setInterVec4NeighborRank(const int& partnerRank_)
{
	std::vector<int>* vecRef = &interXLo;

	auto iter = std::find(&forward_partner[0][0], &forward_partner[0][0] + 6, partnerRank_);
	int distance = std::distance(&forward_partner[0][0], iter);

	switch (distance) {
	case 0:
		// xlo
		vecRef = &interXLo;
		break;
	case 1:
		// xhi
		vecRef = &interXHi;
		break;
	case 2:
		// ylo
		vecRef = &interYLo;
		break;
	case 3:
		// yhi
		vecRef = &interYHi;
		break;
	case 4:
		// zlo
		vecRef = &interZLo;
		break;
	case 5:
		// zhi
		vecRef = &interZHi;
		break;
	default:
		throw std::invalid_argument("Wrong partner id!");
	}

	return vecRef;
}

void Communicator::sendGhosts(const int& partnerRank_,
	                                void*& trandata_)
{
	std::vector<int>* vecRef = setInterVec4NeighborRank(partnerRank_);


	int nParticles = vecRef->size();
	int nData = 1 + (particles->dataPerParticle)*nParticles; // id, (X, V, F) (3) and M, R


	double* sendBuffer = new double[nData];

	int indx = 0;
	
	sendBuffer[indx++] = static_cast<double>(nParticles);
	for (const auto& id : *vecRef)
	{
		std::vector<double> dataI = particles->packParticleData(id);

		for (auto& data : dataI)
			sendBuffer[indx++] = data;
	}
	trandata_ = (void *)sendBuffer;
}

void Communicator::recvGhosts(void*& trandata_) {
	if (trandata_ == nullptr)
		throw std::invalid_argument("The buffer memory has not been initialized yet!");

	double* recvBuffer = static_cast<double*>(trandata_);

	int loc = 0;
	int nParticles = recvBuffer[loc++];
	int particlesRead = 0;



	std::array<double, 3> newX, newV, newF;
	double newR, newM;
	while (particlesRead < nParticles) {
		// reading the data
		std::vector<double> dataI;
		for (int i = 0; i < particles->dataPerParticle; i++)
			dataI.push_back(recvBuffer[loc++]);
		particles->unpackParticleData(dataI);

		// incrementing the number of particles read
		particlesRead++;
	}
		


}


void Communicator::resetOwned()
{
	int originalNlocal;
	int nmax;
	int nlocal = 0;
	int nghosts = 0;
	particles->getNmaxNlocal(nmax, originalNlocal);




	for (int i = 0; i < originalNlocal; i++)
	{
		double x = particles->X(i, 0);
		double y = particles->X(i, 1);
		double z = particles->X(i, 2);

		// owned region.. local region of this rank
		bool xOwned = (x >= myMin[0] && x < myMax[0]);
		bool yOwned = (y >= myMin[1] && y < myMax[1]);
		bool zOwned = (z >= myMin[2] && z < myMax[2]);

		// if it belongs here
		bool Owned = xOwned && yOwned && zOwned;

		if (Owned) {
			particles->swapParticles(i, nlocal);
			nlocal++;
		}
		else
			nghosts++;
	}

	particles->setNmaxNlocal(nmax, nlocal);
	particles->setNGhosts(nghosts);
}

void Communicator::resetInterior()
{	
	int nlocal, nmax;
	particles->getNmaxNlocal(nmax, nlocal);

	interXLo.clear();
	interXHi.clear();
	interYLo.clear();
	interYHi.clear();
	interZLo.clear();
	interZHi.clear();


	double xMinInter = myMin[0] + skin;
	double xMaxInter = myMax[0] - skin;

	double yMinInter = myMin[1] + skin;
	double yMaxInter = myMax[1] - skin;

	double zMinInter = myMin[2] + skin;
	double zMaxInter = myMax[2] - skin;


	for (int i = 0; i < nlocal; i++) {
		double x = particles->X(i, 0);
		double y = particles->X(i, 1);
		double z = particles->X(i, 2);

		// ranges
		// owned region.. local region of this rank
		bool xOwned = (x >= myMin[0] && x < myMax[0]);
		bool yOwned = (y >= myMin[1] && y < myMax[1]);
		bool zOwned = (z >= myMin[2] && z < myMax[2]);
		// interior region .. not a ghost region for neighboring ranks
		bool xInter = (x >= xMinInter && x < xMaxInter);
		bool yInter = (y >= yMinInter && y < yMaxInter);
		bool zInter = (z >= zMinInter && z < zMaxInter);

		// if it belongs here
		bool Owned = xOwned && yOwned && zOwned;

		if (!Owned)
			continue;


		// checking the interior atoms
		if (!xInter && x <= xMinInter)
			interXLo.push_back(i);
		else if (!xInter && x > xMaxInter)
			interXHi.push_back(i);
		if (!yInter && y <= yMinInter)
			interYLo.push_back(i);
		else if (!yInter && y > yMaxInter)
			interYHi.push_back(i);
		if (!zInter && z <= zMinInter)
			interZLo.push_back(i);
		else if (!zInter && z > zMaxInter)
			interZHi.push_back(i);


	}

}

void Communicator::resetGhosts()
{
	int nlocal, nmax, nghosts;
	particles->getNmaxNlocal(nmax, nlocal);
	particles->getNGhosts(nghosts);

	int currentGhostLoc = nlocal;


	double xMinGhost = myMin[0] - skin;
	double xMaxGhost = myMax[0] + skin;

	double yMinGhost = myMin[1] - skin;
	double yMaxGhost = myMax[1] + skin;

	double zMinGhost = myMin[2] - skin;
	double zMaxGhost = myMax[2] + skin;



	for (int i = nlocal; i < nlocal + nghosts; i++) {
		double x = particles->X(i, 0);
		double y = particles->X(i, 1);
		double z = particles->X(i, 2);

		// ranges
		// extended region (ghost region boundaries)
		bool xExtended = (x >= xMinGhost) && (x < xMaxGhost);
		bool yExtended = (y >= yMinGhost) && (y < yMaxGhost);
		bool zExtended = (z >= zMinGhost) && (z < zMaxGhost);

		// if it belongs to the extended region
		bool Extended = xExtended && yExtended && zExtended;

		// it is not a ghost 
		if (!Extended)
			continue;
		particles->swapParticles(i, currentGhostLoc);
		currentGhostLoc++;
	}

	nghosts = currentGhostLoc - nlocal;
	particles->setNGhosts(nghosts);
}


void Communicator::resetParticles()
{
	resetOwned();
	resetGhosts();
	resetInterior();
}

std::array<int,6> Communicator::returnExchangeDests()
{
	std::array<int, 6> output = {
		    neighbor_ranks[0][0],
			neighbor_ranks[0][1],
			neighbor_ranks[1][0],
			neighbor_ranks[1][1],
			neighbor_ranks[2][0],
			neighbor_ranks[2][1]
	};
	return output;
}

int Communicator::getNDests() {
	return nDests;
}

std::vector<double>* Communicator::sendExchangeParticles()
{
	int nlocal, nmax;
	// updating the nmax and nlocal
	particles->getNmaxNlocal(nmax, nlocal);
	// getting the min and max of the box dimension
	std::array<double, 3> min, max;
	box->getDimensions(min, max);

	// resetting the nDests
	nDests = 0;
	// the id of particles to be removed
	// we cannot remove them in the loop
	std::vector<int> ids2beRemoved;

	auto& xRef = particles->xRef();
	auto nsize = static_cast<int>(xRef.size()) / 3;

	for (int i = 0; i < nlocal; i++) {
		// getting particle x, y, z values
		const double x = particles->X(i, 0);
		const double y = particles->X(i, 1);
		const double z = particles->X(i, 2);

		bool removed = false;
		// checking where does it go out of the rank
		if (x < min[0]) {
			removed = true;
			addParticles2ExchangeMessages(0, i);
		}
		else if (x > max[0]) {
			removed = true;
			addParticles2ExchangeMessages(1, i);
		}
		else if (y < min[1]) {
			removed = true;
			addParticles2ExchangeMessages(2, i);
		}
		else if (y > max[1]) {
			removed = true;
			addParticles2ExchangeMessages(3, i);
		}
		else if (z < min[2]) {
			removed = true;
			addParticles2ExchangeMessages(4, i);
		}
		else if (z > max[2]) {
			removed = true;
			addParticles2ExchangeMessages(5, i);
		}

		if (removed == true) {
			nDests++;
			ids2beRemoved.push_back(i);
		}
	}

	// sorting ids
	std::sort(ids2beRemoved.begin(), ids2beRemoved.end(), [&](double a, double b) {return a > b; });
	// removing ids
	for (auto& id : ids2beRemoved)
		particles->removeParticleLocalId(id);


	xRef = particles->xRef();
	nsize = static_cast<int>(xRef.size()) / 3;
	// returning the exchanged messages
	return exchangeMessages;
}

void Communicator::recvExchangeParticles(std::vector<double>& message) {
	int messageSize = static_cast<int>(message.size());
	int nParticles = static_cast<int>(message[0]);
	int nData = 11 * nParticles + 1;
	// int nData = static_cast<int>(message[0]);
	// checking the data container
	if (nData != messageSize) {
		std::cout << "nParticles(" << nParticles << ") and messageSize(" << messageSize << ")" << std::endl;
 		throw std::invalid_argument("Corrupted data container!");
	}
	

	std::array<double, 3> x, v, f;
	double m, r;

	if (nParticles == 0) {
		// This is an empty data container
		return;
	}

	auto& xRef = particles->xRef();
	auto nsize = static_cast<int>(xRef.size()) / 3;
	
	for (int i = 0; i < nParticles; i++)
	{
		// x values
		x[0] = message[11 * i + 1];
		x[1] = message[11 * i + 2];
		x[2] = message[11 * i + 3];
		// v values
		v[0] = message[11 * i + 4];
		v[1] = message[11 * i + 5];
		v[2] = message[11 * i + 6];
		// f values
		f[0] = message[11 * i + 7];
		f[1] = message[11 * i + 8];
		f[2] = message[11 * i + 9];
		// m value
		m = message[11 * i + 10];
		// r values
		r = message[11 * i + 11];
		// adding that data to the particles
		particles->addParticle(x, v, f, m, r);
	}
	// the message has to be cleared so one particle is not added multiple times
	message.clear();
	message.push_back(0.0);

}

void Communicator::addParticles2ExchangeMessages(const int& loc, const int& particleId)
{
	// 3*x, 3*v, 3*f, m, r
	int nDataPerParticle = 11;
	// nData and rest of the data
	//exchangeMessages[loc].reserve(nData);

	// get particle data
	std::array<double, 3> x, v, f;
	double m, r;
	particles->getParticle(particleId, x, v, f, m, r);

	// Adding the number
	exchangeMessages[loc][0] += 1;


	// adding x info
	exchangeMessages[loc].push_back(x[0]);
	exchangeMessages[loc].push_back(x[1]);
	exchangeMessages[loc].push_back(x[2]);
	// adding v info
	exchangeMessages[loc].push_back(v[0]);
	exchangeMessages[loc].push_back(v[1]);
	exchangeMessages[loc].push_back(v[2]);
	// adding f info
	exchangeMessages[loc].push_back(f[0]);
	exchangeMessages[loc].push_back(f[1]);
	exchangeMessages[loc].push_back(f[2]);
	// adding m info
	exchangeMessages[loc].push_back(m);
	// adding r info
	exchangeMessages[loc].push_back(r);
}
