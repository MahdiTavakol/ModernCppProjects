#include "Neighbor.h"
#include "Particles.h"


Neighbor::Neighbor(Engine& engine_) :
	Ref{ engine_,"1" },
	nNeigh{ 0 },
	neighbor_cutoff{ 0.0 }
{}

Neighbor::Neighbor(Engine& engine_, double neighbor_cutoff_) :
	Ref{ engine_,"1" },
	nNeigh{ 0 },
	neighbor_cutoff{ neighbor_cutoff_ }
{}

void Neighbor::init() {
	auto& particles = engine().getParticles();
	int nmax, nlocal;
	particles->getNmaxNlocal(nmax, nlocal);
	nNeigh = nlocal;
	neighList = std::make_unique<int[]>(nlocal * nlocal);
	firstNeigh = std::make_unique<int[]>(nlocal);
	numNeigh = std::make_unique<int[]>(nlocal);
	int neighIndex = 0;
	for (int i = 0; i < nlocal; i++) {
		firstNeigh[i] = neighIndex;
		numNeigh[i] = 0;
		for (int j = 0; j < nlocal; j++) {
			if (i != j) {
				neighList[neighIndex++] = j;
			}
		}
		numNeigh[i] = neighIndex - firstNeigh[i];
	}

}

void Neighbor::getNeighborList(int nNeigh_, int* neighList_, int* firstNeigh_, int* numNeigh_) {
	nNeigh_ = this->nNeigh;
	neighList_ = this->neighList.get();
	firstNeigh_ = this->firstNeigh.get();
	numNeigh_ = this->numNeigh.get();
}