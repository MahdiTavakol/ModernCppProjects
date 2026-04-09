#pragma once
#include "Comm_strategy.h"
#include <mpi.h>
#include <vector>

constexpr int maxRequest = 10;

class MPI_comm_strategy : public Comm_strategy
{
public:
	MPI_comm_strategy();
	void update(double* buff_, const int size_, const int tag, const int src_, const int dst_) override;
	virtual void send(double* buffer_, const int size_, const int dst_, const int tag_) override;
	virtual void recv(double* buffer_, const int size_, const int src_, const int tag_) override;
	virtual void waitAll() override;

private:
	MPI_Comm world;
	int num_request = 0;
	MPI_Request requests[maxRequest];
	int max_tag;
};
