#pragma once

#include "Ref.h"
#include "Engine.h"

class Neighbor : public Ref {
public:
	Neighbor(Engine& engine_);
	Neighbor(Engine& engine_, double neighbor_cutoff_);
	virtual void init();
	virtual void update() {}
	void getNeighborList(int nNeigh_, int* neighList_, int* firstNeigh_, int* numNeigh_);

protected:
	int nNeigh;
	std::unique_ptr<int[]> neighList, numNeigh, firstNeigh;
	double neighbor_cutoff;
};