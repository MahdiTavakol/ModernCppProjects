#include "test.hpp"
#include "catch_amalgamated.hpp"

TEST_CASE("Testing the damper")
{
	std::cout << "Testing the damper" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// input args
	constexpr int nmax = 5;
	constexpr int nlocal = 5;
	std::vector<double> x;
	std::vector<double> v = {
		120.75,  -45.30,   88.92,     // particle 0
	   -210.40,  175.60,  -25.15,     // particle 1
		 15.25,    9.80,  315.47,     // particle 2
		 -8.50, -300.75,  920.33,     // particle 3
		260.10,    1.25,  178.64      // particle 4
	};
	std::vector<double> f;
	std::vector<double> m;
	std::vector<double> r;
	x.resize(3 * nlocal);
	f.resize(3 * nlocal);
	m.resize(3 * nlocal);
	r.resize(3 * nlocal);

	// connected atoms
	std::vector<Connection> connectionInfo;
	connectionInfo.push_back(
		Connection(0, 1, 1.0)
	);
	connectionInfo.push_back(
		Connection(2, 4, 5.0)
	);

	// The spring object
	std::unique_ptr<DamperField> damper_ptr =
		std::make_unique<DamperField>(connectionInfo);
	// the particles object
	std::unique_ptr<Particles> particles_ptr =
		std::make_unique<MockedParticles>(nmax, x, v, f, r, m);
	// input args
	std::vector<std::unique_ptr<Ref>> inputs;
	inputs.push_back(std::move(damper_ptr));
	inputs.push_back(std::move(particles_ptr));
	// The test fixture
	EngineFixture engineFixture(inputs);
	// getting the engine
	auto engine_ptr = engineFixture.returnEngine();
	// getting the forcefield object
	auto& springRef = engine_ptr->getForceField();
	// checking the springRef
	REQUIRE(springRef);
	// updating the forces
	springRef->update();
	// getting the particles
	auto& particlesRef = engine_ptr->getParticlesForUpdate();
	// checking it 
	REQUIRE(particlesRef);
	// getting the force values
	auto Fs = particlesRef->getFData();
	// checking the pointer
	REQUIRE(Fs);
	// expected values
	std::vector<double> expectedFs =
	{
		-331.15, 220.90,-114.07,  // Particle 0
		 331.15,-220.90, 114.07,  // Particle 1
		1224.25, -42.75,-684.15,  // Particle 2
		   0.00,   0.00,   0.00,  // Particle 3
	   -1224.25,  42.75, 684.15   // Particle 4
	};
	// Checking the results
	for (int i = 0; i < 3 * nlocal; i++)
		REQUIRE_THAT(Fs[i], Catch::Matchers::WithinAbs(expectedFs[i], 1e-6));


}