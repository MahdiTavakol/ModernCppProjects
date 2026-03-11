#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iostream>

#include "test.hpp"




TEST_CASE("Testing the simple neighbor class")
{
	std::cout << "Testing the simple neighbor class" << std::endl;
	std::cout << std::string(80, '=') << std::endl;


	// the cutoff value
	constexpr double cutoff = 70.0;

	std::vector<double> x =
	{
		-280.0, 0.0, 0.0,
		-260.0,5.0,0.0,
		-240.0,-5.0,10.0,
		-50.0,0.0,0.0,
		-30.0,10.0,0.0,
		-10.0,-5.0,0.0,
		15.0,0.0,5.0,
		40.0,-5.0,0.0,
		200.0,0.0,0.0,
		235.0,0.0,0.0
	};

	std::vector<double> v(x.size(), 0.0);
	std::vector<double> f(x.size(), 0.0);
	std::vector<double> r(x.size()/3, 0.0);
	std::vector<double> m(x.size()/3, 0.0);

	// expected results
	int expectedNNeigh = 10;
	std::vector<int> expectedNeighList = {
		1,2,
		0,2,
		0,1,
		4,5,6,
		3,5,6,
		3,4,
		3,4,5,7,
		5,6,
		9,
		8
	};
	std::vector<int> expectedFirstNeigh = {
		0,
		2,
		4,
		6,
		9,
		12,
		14,
		18,
		20,
		21
	};
	std::vector<int> expectedNumNeighs = {
		2,
		2,
		2,
		3,
		3,
		2,
		4,
		2,
		1,
		1
	};

	// box dimensions
	array<double, 3> min = { -300.0,-300.0,-300.0 };
	array<double, 3> max = { 300.0, 300.0, 300.0 };
	// box
	std::unique_ptr<Box> box = std::make_unique<Box>(min, max);
	// creating the mockedParticles object
	int nmax = 10;
	int nlocal = 10;
	std::unique_ptr<Particles> mockedParticles =
		std::make_unique<MockedParticles>(nmax, x, v, f, r, m);
	// building the neighborlist
	std::unique_ptr<Neighbor> neighborList = std::make_unique<SimpleNeighbor>(cutoff);
	// putting everything into the engine
	std::vector<std::unique_ptr<Ref>> inputs;
	inputs.push_back(std::move(mockedParticles));
	inputs.push_back(std::move(box));
	inputs.push_back(std::move(neighborList));
	// creating the engine fixture
	EngineFixture engineFixture(inputs);
	// getting the engine_ptr
	auto engine_ptr = engineFixture.returnEngine();
	// getting the neighborlist
	auto& neighborListRef = engine_ptr->getNeighbor();
	// checking the pointer
	REQUIRE(neighborListRef);
	// getting the results
	int nNeigh = 0;
	int* neighList = nullptr;
	int* firstNeigh = nullptr;
	int* numNeigh = nullptr;
	neighborListRef->getNeighborList(nNeigh, neighList, firstNeigh, numNeigh);
	// checking the results
	REQUIRE(nNeigh == expectedNNeigh);
	// 
	for (int i = 0; nNeigh; i++) {
		REQUIRE(neighList[i] == expectedNeighList[i]);
		REQUIRE(firstNeigh[i] == expectedFirstNeigh[i]);
		REQUIRE(numNeigh[i] == expectedNumNeighs[i]);
	}
}

