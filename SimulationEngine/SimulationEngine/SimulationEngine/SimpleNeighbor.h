#pragma once

#include "Neighbor.h"

class SimpleNeighbor : public Neighbor
{
public:
	SimpleNeighbor(Engine& engine_, std::vector<std::string> args_);
	SimpleNeighbor(Engine& engine_, double neighbor_cutoff_);
	void init() override;
	void update() override;

private:
	double distance(const double* const x, const int& i, const int& j);
};