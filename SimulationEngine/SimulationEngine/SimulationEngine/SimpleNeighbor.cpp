#include "SimpleNeighbor.h"
#include "Particles.h"

SimpleNeighbor::SimpleNeighbor(Engine& engine_, std::vector<std::string> args_) :
	Neighbor{ engine_, args_ }
{}

SimpleNeighbor::SimpleNeighbor(Engine& engine_, double neighbor_cutoff_) :
	Neighbor{ engine_, neighbor_cutoff_ }
{}

void SimpleNeighbor::init() {
	this->update();
}

void SimpleNeighbor::update() {
	auto& P = engine().getParticles();
	int nmax, nlocal;
	P->getNmaxNlocal(nmax, nlocal);
	auto x = P->getXData();
	nNeigh = nlocal;
	neighList = std::make_unique<int[]>(nlocal * nlocal);
	firstNeigh = std::make_unique<int[]>(nlocal);
	numNeigh = std::make_unique<int[]>(nlocal);
	int neighIndex = 0;
	for (int i = 0; i < nlocal; i++) {
		firstNeigh[i] = neighIndex;
		numNeigh[i] = 0;
		for (int j = 0; j < nlocal; j++) {
			if (i == j) continue;
			double r = distance(x, i, j);
			if (r <= neighbor_cutoff) {
					neighList[neighIndex++] = j;
			}
		}
		numNeigh[i] = neighIndex - firstNeigh[i];
	}
}

double SimpleNeighbor::distance(const double* const x, const int& i, const int& j) {
	double r2 = (x[3 * i] - x[3 * j]) * (x[3 * i] - x[3 * j]) +
		(x[3 * i + 1] - x[3 * j + 1]) * (x[3 * i + 1] - x[3 * j + 1]) +
		(x[3 * i + 2] - x[3 * j + 2]) * (x[3 * i + 2] - x[3 * j + 2]);
	return std::sqrt(r2);
}