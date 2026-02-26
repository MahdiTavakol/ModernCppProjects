#include "Neighbor.h"
#include "Particles.h"
#include "Error.h"

Neighbor::Neighbor(Engine& engine_, std::vector<std::string> args_):
	Ref{ engine_, "1" }
{
	int nargs = args_.size();
	auto& error = engine().getError();
	if (nargs < 3) {
		error->one("Not enough arguments for neighbor command! Expected at least 2, got " + std::to_string(nargs));
		neighbor_cutoff = 0.0;
		return;
	}
	neighbor_cutoff = std::stod(args_[2]);
}

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

void Neighbor::injectDependencies(Engine& engine_) {
	Ref::injectDependencies(engine_);
	particles = engine_.getParticlesForUpdate().get();
}

void Neighbor::init() {
	int nmax, nlocal;
	particles->getNmaxNlocal(nmax, nlocal);
	nNeigh = nlocal;
	neighList.resize(nlocal * nlocal);
	firstNeigh.reserve(nlocal);
	numNeigh.reserve(nlocal);
	int neighIndex = 0;
	for (int i = 0; i < nlocal; i++) {
		firstNeigh.push_back(neighIndex);
		for (int j = 0; j < nlocal; j++) {
			if (i != j) {
				neighList[neighIndex++] = j;
			}
		}
		numNeigh.push_back(neighIndex - firstNeigh[i]);
	}

}

void Neighbor::getNeighborList(int& nNeigh_, int*& neighList_, int*& firstNeigh_, int*& numNeigh_) {
	nNeigh_ = this->nNeigh;
	neighList_ = this->neighList.data();
	firstNeigh_ = this->firstNeigh.data();
	numNeigh_ = this->numNeigh.data();
}