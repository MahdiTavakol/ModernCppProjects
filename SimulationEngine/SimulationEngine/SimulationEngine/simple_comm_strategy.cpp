#include "Simple_comm_strategy.h"

Simple_comm_strategy::Simple_comm_strategy():
	Comm_strategy{ 0 }
{}

Simple_comm_strategy::Simple_comm_strategy(const int& rank_) :
	Comm_strategy{ rank_ }
{}

void Simple_comm_strategy::update(double* buff_, const int size_, const int tag_, const int src_, const int dst_)
{
	// nothing to do here
}

void Simple_comm_strategy::send(double* buff_, const int size_, const int dst_, const int tag_)
{
	// nothing to do here
}

void Simple_comm_strategy::recv(double* buffer_, const int size_, const int src_, const int tag_)
{
	// nothing to do here
}

void Simple_comm_strategy::waitAll()
{
	// nothing to do here
}

void Simple_comm_strategy::reduceAll(double* one_, double* all_, const int size_)
{
	// nothing to do here
}

void Simple_comm_strategy::reduceAll(int* one_, int* all_, const int size_)
{
	// nothing to do here
}

std::unique_ptr<Comm_strategy> Simple_comm_strategy::split(const int maxRanks_)
{
	std::unique_ptr<Comm_strategy> out = std::make_unique<Simple_comm_strategy>(*this);
	return out;
}
