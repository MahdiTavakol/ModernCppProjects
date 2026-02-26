#include "catch_amalgamated.hpp"
#include "test.hpp"
#include <iostream>


// integrator unittest
TEST_CASE("Testing the integrator class update positions") {
	std::cout << "Testing the integrator class update positions" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// creating the engine
	Engine engine;
	// put some particles.
	constexpr int nmax = 5;
	constexpr int nlocal = 5;
	std::vector<double> x = {
		   100.0,30.0,40.0, // particle 0
		  -150.0,200.0,-10.0, // particle 1
		   5.0,7.5,200.95, // particle 2
		   0.0,-349.5,1000.54, // particle 3
		   230.5,0.95,145.78, // particle 4
	};
	std::vector<double> v = {
	  42.73,  -88.19,   15.64,   // particle 0
	 -73.52,   94.07,  -36.28,   // particle 1
	  11.95,  -59.83,   78.44,   // particle 2
	 -98.61,   23.17,  -44.92,   // particle 3
	  67.08,  -12.54,   99.31,   // particle 4
	};
	std::vector<double> f;
	std::vector<double> r;
	std::vector<double> m;
	f.resize(3 * nlocal);
	r.resize(3 * nlocal);
	m.resize(3 * nlocal);
	std::unique_ptr<Particles> mockedParticles =
		std::make_unique<MockedParticles>(engine, nmax, x, v, f, r, m);
	// the integrator instance
	constexpr double dt = 1.0;
	std::unique_ptr<Integrator> integrator =
		std::make_unique<MockedIntegrator>(engine);
	integrator->setDt(dt);
	// registering Particles and Integrator objects
	engine.setItem(std::move(integrator));
	engine.setItem(std::move(mockedParticles));
	// injecting dependencies
	engine.injectDependencies();
	// calling the update_positions function of the integrator
	auto& integratorRef = engine.getIntegrator();
	REQUIRE(integratorRef);
	MockedIntegrator* integratorRaw = dynamic_cast<MockedIntegrator*>(integratorRef.get());
	REQUIRE(integratorRaw);
	integratorRaw->updateX();
	// extracting the results
	auto& particlesRef = engine.getParticles();
	REQUIRE(particlesRef);
	double* Xs = particlesRef->getXData();
	REQUIRE(Xs);
	// expected results
	std::vector<double> expectedXs;
	REQUIRE(x.size() == v.size());
	for (int i = 0; i < x.size(); i++)
		expectedXs.push_back(x[i] + dt * v[i]);

	// checking
	for (int i = 0; i < x.size(); i++)
		REQUIRE_THAT(Xs[i], Catch::Matchers::WithinAbs(expectedXs[i], 1e-6));
}

// integrator unittest
TEST_CASE("Testing the integrator class update velocities") {
	std::cout << "Testing the integrator class update velocities" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// creating the engine
	Engine engine;
	// put some particles.
	constexpr int nmax = 5;
	constexpr int nlocal = 5;
	std::vector<double> x;
	x.resize(nlocal * 3);
	std::vector<double> v = {
	  42.73,  -88.19,   15.64,   // particle 0
	 -73.52,   94.07,  -36.28,   // particle 1
	  11.95,  -59.83,   78.44,   // particle 2
	 -98.61,   23.17,  -44.92,   // particle 3
	  67.08,  -12.54,   99.31,   // particle 4
	};
	std::vector<double> f = {
	  3.72,  -8.15,   1.94,   // particle 0
	 -6.48,   9.03,  -2.67,   // particle 1
	  0.58,  -4.91,   7.36,   // particle 2
	 -9.27,   2.14,  -5.83,   // particle 3
	  8.61,  -1.32,   4.75,   // particle 4
	};
	std::vector<double> r;
	r.resize(3 * nlocal);
	std::vector<double> m = { 5,5,5,10,10 };
	std::unique_ptr<Particles> mockedParticles =
		std::make_unique<MockedParticles>(engine, nmax, x, v, f, r, m);
	// the integrator instance
	constexpr double dt = 1.0;
	std::unique_ptr<Integrator> integrator =
		std::make_unique<MockedIntegrator>(engine);
	integrator->setDt(dt);
	// registering Particles and Integrator objects
	engine.setItem(std::move(integrator));
	engine.setItem(std::move(mockedParticles));
	// injecting dependencies
	engine.injectDependencies();
	// calling the update_positions function of the integrator
	auto& integratorRef = engine.getIntegrator();
	REQUIRE(integratorRef);
	MockedIntegrator* integratorRaw = dynamic_cast<MockedIntegrator*>(integratorRef.get());
	REQUIRE(integratorRaw);
	integratorRaw->updateX();
	// extracting the results
	auto& particlesRef = engine.getParticles();
	REQUIRE(particlesRef);
	double* Vs = particlesRef->getVData();
	REQUIRE(Vs);
	// expected results
	std::vector<double> expectedVs;
	REQUIRE(v.size() == f.size());
	REQUIRE(f.size() == m.size());
	for (int i = 0; i < x.size(); i++)
		expectedVs.push_back(v[i] + dt * f[i] / m[i]);

	// checking
	for (int i = 0; i < x.size(); i++)
		REQUIRE_THAT(Vs[i], Catch::Matchers::WithinAbs(expectedVs[i], 1e-6));
}
