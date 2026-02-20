#pragma once

#include "Type.h"
#include "Engine.h"

class Neighbor : public Type {
public:
	Neighbor(Engine& engine_, double neighbor_cutoff_) :
		Type{ engine_,"1" },
		nNeigh{ 0 },
		neighbor_cutoff{ neighbor_cutoff_ }
	{}
	virtual void init();
	virtual void update() {}
	void getNeighborList(int nNeigh_, int* neighList_, int* firstNeigh_, int* numNeigh_);

protected:
	int nNeigh;
	std::unique_ptr<int[]> neighList, numNeigh, firstNeigh;
	double neighbor_cutoff;
};