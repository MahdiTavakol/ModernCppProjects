#include "catch_amalgamated.hpp"
#include "test.hpp"
#include <iostream>
#include <array>
#include <vector>

using std::array;
using std::vector;

// unittest for collintegrator
TEST_CASE("Testing the collision integrator")
{
	std::cout << "Testing the collision integrator" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// creating the engine
	Engine engine;
	// creating the mockedNeighbor object
	int nNeigh = 5;
	// Each particle starts at a different offset
	std::vector<int> firstNeighVec = { 0, 3, 4, 7, 9 };
	// Different number of neighbors per particle
	std::vector<int> numNeighVec = { 3, 1, 3, 2, 3 };
	// Flat neighbor list storage
	std::vector<int> neighListVec = {
		1, 2, 4,      // particle 0 (3 neighbors)
		0,            // particle 1 (1 neighbor)
		0, 3, 4,      // particle 2 (3 neighbors)
		2, 4,         // particle 3 (2 neighbors)
		0, 2, 3       // particle 4 (3 neighbor)
	};
	std::unique_ptr<Neighbor> mockedNeighbor =
		std::make_unique<MockedNeighbor>(engine, nNeigh, neighListVec, firstNeighVec, numNeighVec);
	// creating the mockedParticles object
	// put some particles.
	int nmax = 5;
	std::vector<double> x = {
		   150.0,80.0,50.0, // particle 0
		   150.0,180.0,0.0, // particle 1
		   0.0,0.0,0.0, // particle 2
		   0.0,-200.0,0.0, // particle 3
		   200.0,0.0,0.0, // particle 4
	};
	std::vector<double> v = {
		  0.0,0.0,0.0, // particle 0
		  0.0,0.0,0.0, // particle 1
		  1.0,0.0,0.0, // particle 2
		  0.0,-1.0,0.0, // particle 3
		 -1.0,0.0,0.0, // particle 4
	};
	std::vector<double> f = {
		  0.0,0.0,0.0, // particle 0
		  0.0,0.0,0.0, // particle 1
		  0.0,0.0,0.0, // particle 2
		  0.0,0.0,0.0, // particle 3
		  0.0,0.0,0.0, // particle 4
	};
	std::vector<double> r = { 1,10,100,100,100 };
	std::vector<double> m = { 5,5,5,10,10 };
	std::unique_ptr<Particles> mockedParticles =
		std::make_unique<MockedParticles>(engine, nmax, x, v, f, r, m);
	// creating the collision object under test
	std::unique_ptr<Integrator> colliIntegrator = std::make_unique<ColliIntegrator>(engine);
	// registering the items
	engine.setItem(std::move(mockedNeighbor));
	engine.setItem(std::move(mockedParticles));
	engine.setItem(std::move(colliIntegrator));
	// getting the integrator from the engine
	auto& integratorRef = engine.getIntegrator();
	// checking if the engine has integrator
	REQUIRE(integratorRef);
	// running one integration step
	integratorRef->post_force();
	// getting the particle coordinates from the engine
	auto& particlesRef = engine.getParticles();
	// checking if the engine has particles
	REQUIRE(particlesRef);
	// getting particle coordinates
	double* Xs = particlesRef->getXData();
	// getting particle velocities 
	double* Vs = particlesRef->getVData();
	// getting particle forces
	double* Fs = particlesRef->getFData();



	vector<double> expectedVs = {
				  0.0,0.0,0.0, // particle 0
				  0.0,0.0,0.0, // particle 1
				 -1.66666666666666666,0.0,0.0, // particle 2
				  0.0,-1.0,0.0, // particle 3
				  0.33333333333333333,0.0,0.0, // particle 4
	};

	vector<double> expectedXs = {
				   150.0,80.0,50.0, // particle 0
				   150.0,180.0,0.0, // particle 1
				  -1.6666666666666666,0.0,0.0, // particle 2
				   0.0,-201.0,0.0, // particle 3
				   200.333333333333333,0.0,0.0, // particle 4
	};

	vector<double> expectedFs = {
				  0.0,0.0,0.0, // particle 0
				  0.0,0.0,0.0, // particle 1
				  0.0,0.0,0.0, // particle 2
				  0.0,0.0,0.0, // particle 3
				  0.0,0.0,0.0, // particle 4
	};


	for (int i = 0; i < 3 * 5; i++) {
		REQUIRE_THAT(Xs[i], Catch::Matchers::WithinAbs(expectedXs[i], 1e-6));
		REQUIRE_THAT(Vs[i], Catch::Matchers::WithinAbs(expectedVs[i], 1e-6));
		REQUIRE_THAT(Fs[i], Catch::Matchers::WithinAbs(expectedFs[i], 1e-6));
	}
}
