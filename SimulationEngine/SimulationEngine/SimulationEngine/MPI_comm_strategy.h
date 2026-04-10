#pragma once
#include "Comm_strategy.h"
#include <mpi.h>
#include <vector>
#include <memory>

constexpr int maxRequest = 10;

class MPI_comm_strategy : public Comm_strategy
{
public:
	MPI_comm_strategy();
	MPI_comm_strategy(MPI_Comm world_, const int& rank_);
	void update(double* buff_, const int size_, const int tag, const int src_, const int dst_) override;
	virtual void send(double* buffer_, const int size_, const int dst_, const int tag_) override;
	virtual void recv(double* buffer_, const int size_, const int src_, const int tag_) override;
	virtual void waitAll() override;
	virtual void reduceAll(double* one_, double* all_, const int size_) override;
	virtual void reduceAll(int* one_, int* all_, const int size_) override;
	virtual std::unique_ptr<Comm_strategy> split(const int maxRanks_) override;
	MPI_Comm* getWorldPtr() { return &world; }

private:
	MPI_Comm world;
	int num_request = 0;
	MPI_Request requests[maxRequest];
	int max_tag;
};
