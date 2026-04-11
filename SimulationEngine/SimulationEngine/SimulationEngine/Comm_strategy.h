#pragma once
#include "Ref.h"

#include <memory>

class Comm_strategy : public Ref 
{
public:
	Comm_strategy() = default;
	Comm_strategy(const int& rank_);
	int getRank();
	int getSize();

	virtual void update(double* buffer_,const int size_, const int tag_, const int src_, const int dst_) = 0;
	virtual void send(double* buffer_, const int size_, const int dst_, const int tag_) = 0;
	virtual void recv(double* buffer_, const int size_, const int src_, const int tag_) = 0;
	virtual void waitAll() = 0;
	virtual void reduceAll(double* one_, double* all_, const int size_) = 0;
	virtual void reduceAll(int* one_, int* all_, const int size_) = 0;
	virtual std::unique_ptr<Comm_strategy> split(const int maxRanks_) = 0;

protected:
	int rank, size;

};
