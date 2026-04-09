#include "test.hpp"
#include "test_communicator_helpers.hpp"
#include "Comm_strategy.h"
#include "MPI_comm_strategy.h"


static void minRanksRequirement(std::unique_ptr<Comm_strategy>& comm_strategy_, const int minSize_)
{
	int size = comm_strategy_->getSize();
	std::string warningTest = std::string("This test requires at least ") + std::to_string(minSize_) + std::string(" ranks!");
	if (size < minSize_)
		SKIP(warningTest.c_str());
}

TEST_CASE("Testing updating a buffer MPI_comm_strategy","[mpi]")
{
	std::cout << "Testing updating a buffer through MPI_comm_strategy" << std::endl;
	std::cout << std::string(80, '=');

	
	std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();

	std::vector<double> buffer;
	const int nData = 10;

	int rank = comm_strategy->getRank();
	int size = comm_strategy->getSize();

	minRanksRequirement(comm_strategy, 3);


	if (rank == 0)
	{
		buffer = {
			-3.12, 7.45, 0.98, -1.76, 5.33,
			-6.21, 2.44, 8.90, -0.55, 4.11
		};
	}
	else if (rank == 2)
	{
		buffer.resize(nData);
	}

	std::vector<double> expectedBuffer = {
	-3.12, 7.45, 0.98, -1.76, 5.33,
	-6.21, 2.44, 8.90, -0.55, 4.11
	};
	std::vector<double> receivedBuffer;
	receivedBuffer.resize(buffer.size());


	comm_strategy->update(buffer.data(), nData, 0, 0, 2);

	if (rank == 2)
		REQUIRE_THAT(buffer.data(), Array1DMatcher(expectedBuffer.data(), expectedBuffer.size(), 1e-6));
	else
		SUCCEED("Nothing to check here!");
}

TEST_CASE("Testing sending/receiving a buffer to another buffer with MPI", "[mpi]")
{
	std::cout << "Testing sending/receiving a buffer to another buffer with MPI" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	
	std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();

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


	if (rank == 1)
	{
		sendBuffer = {
			3.14, -7.82, 0.56, 12.47,
			-2.91, 8.33, -0.44, 5.78,
			9.01, -6.25, 1.99, -3.67
		};
	}
	else if (rank == 2)
	{
		expectedRecvBuffer = {
			3.14, -7.82, 0.56, 12.47,
			-2.91, 8.33, -0.44, 5.78,
			9.01, -6.25, 1.99, -3.67
		};
	}

	int tag = 0;
	if (rank == 1)
		comm_strategy->send(sendBuffer.data(), sendBuffer.size(),2,tag);
	if (rank == 2)
		comm_strategy->recv(recvBuffer.data(), recvBuffer.size(), 1, tag);

	// checking the results
	if (rank == 2)
		REQUIRE_THAT(recvBuffer.data(), Array1DMatcher(expectedRecvBuffer.data(), expectedRecvBuffer.size(), 1e-6));
	else
		SUCCEED("Nothing to check here!");

}