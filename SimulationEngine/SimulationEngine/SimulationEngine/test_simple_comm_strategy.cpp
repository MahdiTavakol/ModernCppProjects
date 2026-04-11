#include "test.hpp"
#include "test_communicator_helpers.hpp"
#include "Comm_strategy.h"
#include "Simple_comm_strategy.h"


// To do::: add tests for a simple_comm_strategy class 
// to be the default comm_strategy for the engine which is the serial comm_strategy

TEST_CASE("Testing updating a buffer simple_comm_strategy", "[mpi]")
{
	printRankZero("Testing updating a buffer through simple_comm_strategy");
	printRankZero(std::string(80, '='));


	std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<Simple_comm_strategy>();

	std::vector<double> buffer;
	const int nData = 10;

	int rank = comm_strategy->getRank();
	int size = comm_strategy->getSize();

	minRanksRequirement(comm_strategy, 3);


	comm_strategy->update(buffer.data(), nData, 0, 0, 2);

	SUCCEED("Nothing to check here!");
}

TEST_CASE("Testing sending/receiving a buffer to another buffer with MPI", "[mpi]")
{
	printRankZero("Testing sending/receiving a buffer to another buffer with MPI");
	printRankZero(std::string(80, '='));

	std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<Simple_comm_strategy>();

	int size, rank;
	rank = comm_strategy->getRank();
	size = comm_strategy->getSize();

	minRanksRequirement(comm_strategy, 4);

	const int nData = 12;
	std::vector<double> sendBuffer;
	std::vector<double> recvBuffer;
	std::vector<double> expectedRecvBuffer;

	sendBuffer.resize(nData);
	recvBuffer.resize(nData);
	expectedRecvBuffer.resize(nData);


	int tag = 0;
	if (rank == 1)
		comm_strategy->send(sendBuffer.data(), sendBuffer.size(), 2, tag);
	if (rank == 2)
		comm_strategy->recv(recvBuffer.data(), recvBuffer.size(), 1, tag);

	// checking the results
	SUCCEED("Nothing to check here!");
}
