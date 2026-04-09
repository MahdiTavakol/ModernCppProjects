#include "test.hpp"
#include "test_communicator_helpers.hpp"
#include "Comm_strategy.h"
#include "MPI_comm_strategy.h"


TEST_CASE("Testing sending a buffer through MPI_comm_strategy","[mpi]")
{
	std::cout << "Testing MPI_comm_strategy" << std::endl;
	std::cout << std::string(80, '=');

	
	std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();

	std::vector<double> buffer = {
	-3.12, 7.45, 0.98, -1.76, 5.33,
	-6.21, 2.44, 8.90, -0.55, 4.11
	};
	std::vector<double> expectedBuffer = {
	-3.12, 7.45, 0.98, -1.76, 5.33,
	-6.21, 2.44, 8.90, -0.55, 4.11
	};

	int nData = 8;

	double* sendPtr, * recvPtr;
	sendPtr = new double[nData];
	recvPtr = new double[nData];

	int rank = comm_strategy->getRank();
	int size = comm_strategy->getSize();

	std::cout << "Rank=" << rank << "," << "size=" << size << std::endl;


	delete[] sendPtr;
	delete[] recvPtr;
}