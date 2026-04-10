#include "Comm_strategy.h"

Comm_strategy::Comm_strategy(const int& rank_):
	rank{rank_}
{}

int Comm_strategy::getRank()
{
	return rank;
}

int Comm_strategy::getSize()
{
	return size;
}