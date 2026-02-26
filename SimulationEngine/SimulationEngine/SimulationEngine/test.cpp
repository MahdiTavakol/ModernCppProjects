#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iostream>

#include "test.hpp"




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
		expectedVs.push_back(v[i] + dt * f[i]/m[i]);

	// checking
	for (int i = 0; i < x.size(); i++)
		REQUIRE_THAT(Vs[i], Catch::Matchers::WithinAbs(expectedVs[i], 1e-6));
}

// integrator unittest for orchasterating fixes
TEST_CASE("Integrator unittest for orchasterating fixes")
{
	std::cout << "Integrator unittest for orchasterating fixes" << std::endl;
	std::cout << std::string(80,'=') << std::endl;
	// 
	constexpr int nInit = 8;
	constexpr int nSetup = 5;
	constexpr int nInitInt = 10;
	constexpr int nPreForce = 7;
	constexpr int nPostForce = 13;
	constexpr int nFinInt = 3;
	// building the mocked engine
	Engine engine;
	// building the mockedFixes
	std::unique_ptr<Fix> fix1 = std::make_unique<MockedFix>(engine, FixMask::INIT_INTEGRATE,"1");
	std::unique_ptr<Fix> fix2 = std::make_unique<MockedFix>(engine, FixMask::PRE_FORCE,"2");
	std::unique_ptr<Fix> fix3 = std::make_unique<MockedFix>(engine, FixMask::POST_FORCE,"3");
	std::unique_ptr<Fix> fix4 = std::make_unique<MockedFix>(engine, FixMask::FINAL_INTEGRATE,"4");
	// registering those fixes
	engine.setItem(std::move(fix1));
	engine.setItem(std::move(fix2));
	engine.setItem(std::move(fix3));
	engine.setItem(std::move(fix4));
	// creating the integrator
	std::unique_ptr<Integrator> integrator = std::make_unique<Integrator>(engine);
	// regisetering the integrator
	engine.setItem(std::move(integrator));
	// the rest of required mocks
	std::unique_ptr<Neighbor> mockedNeighbor = std::make_unique<MockedNeighbor>(engine);
	std::unique_ptr<ForceField> mockedForceField = std::make_unique<MockedForceField>(engine);
	engine.setItem(std::move(mockedNeighbor));
	engine.setItem(std::move(mockedForceField));
	// returning the integrator and mocked fixes
	auto& integratorRef = engine.getIntegrator();
	auto& fix1ref = engine.returnFixById("1");
	auto& fix2ref = engine.returnFixById("2");
	auto& fix3ref = engine.returnFixById("3");
	auto& fix4ref = engine.returnFixById("4");
	// checking if they are present
	REQUIRE(integratorRef);
	REQUIRE(fix1ref);
	REQUIRE(fix2ref);
	REQUIRE(fix3ref);
	REQUIRE(fix4ref);
	// coverting the fixrefs
	MockedFix* fix1mockRef = dynamic_cast<MockedFix*>(fix1ref.get());
	MockedFix* fix2mockRef = dynamic_cast<MockedFix*>(fix2ref.get());
	MockedFix* fix3mockRef = dynamic_cast<MockedFix*>(fix3ref.get());
	MockedFix* fix4mockRef = dynamic_cast<MockedFix*>(fix4ref.get());
	// checking the mockedfixrefs
	REQUIRE(fix1mockRef);
	REQUIRE(fix2mockRef);
	REQUIRE(fix3mockRef);
	REQUIRE(fix4mockRef);
	// calling the differnt functions various times.
	for (int i = 0; i < nInit; i++)
		integratorRef->init();
	for (int i = 0; i < nSetup; i++)
		integratorRef->setup();
	for (int i = 0; i < nInitInt; i++)
		integratorRef->initial_integrate();
	for (int i = 0; i < nPreForce; i++)
		integratorRef->pre_force();
	for (int i = 0; i < nPostForce; i++)
		integratorRef->post_force();
	for (int i = 0; i < nFinInt; i++)
		integratorRef->final_integrate();
	// checking the output
	REQUIRE(fix1mockRef->nInit == nInit);
	REQUIRE(fix2mockRef->nInit == nInit);
	REQUIRE(fix3mockRef->nInit == nInit);
	REQUIRE(fix4mockRef->nInit == nInit);
	REQUIRE(fix1mockRef->nSetup == nSetup);
	REQUIRE(fix2mockRef->nSetup == nSetup);
	REQUIRE(fix3mockRef->nSetup == nSetup);
	REQUIRE(fix4mockRef->nSetup == nSetup);
	REQUIRE(fix1mockRef->nTimes == nInitInt);
	REQUIRE(fix2mockRef->nTimes == nPreForce);
	REQUIRE(fix3mockRef->nTimes == nPostForce);
	REQUIRE(fix4mockRef->nTimes == nFinInt);
}


// unittest for the fix orchasterator
TEST_CASE("testing the fix orchasterator class")
{
	std::cout << "testing the fix orchasterator class" << std::endl;
	std::cout << std::string(80, '=');
	// constant parameters
	constexpr int nInit = 8;
	constexpr int nSetup = 5;
	constexpr int nInitInt = 10;
	constexpr int nPreForce = 7;
	constexpr int nPostForce = 13;
	constexpr int nFinInt = 3;
	// building the mocked engine
	Engine engine;
	// building the mockedFixes
	std::unique_ptr<Fix> fix1 = std::make_unique<MockedFix>(engine, FixMask::INIT_INTEGRATE, "1");
	std::unique_ptr<Fix> fix2 = std::make_unique<MockedFix>(engine, FixMask::PRE_FORCE, "2");
	std::unique_ptr<Fix> fix3 = std::make_unique<MockedFix>(engine, FixMask::POST_FORCE, "3");
	std::unique_ptr<Fix> fix4 = std::make_unique<MockedFix>(engine, FixMask::FINAL_INTEGRATE, "4");
	// creating the integrator
	std::unique_ptr<FixList> fixList = std::make_unique<FixList>(engine);
	// registering those fixes to the fixList
	fixList->addFix(std::move(fix1));
	fixList->addFix(std::move(fix2));
	fixList->addFix(std::move(fix3));
	fixList->addFix(std::move(fix4));
	// regisetering the for list
	engine.setItem(std::move(fixList));
	// returning the fixList and mocked fixes
	auto& fixListRef = engine.getFixListObj();
	auto& fix1ref = fixListRef->returnFixById("1");
	auto& fix2ref = fixListRef->returnFixById("2");
	auto& fix3ref = fixListRef->returnFixById("3");
	auto& fix4ref = fixListRef->returnFixById("4");
	// checking if they are present
	REQUIRE(fixListRef);
	REQUIRE(fix1ref);
	REQUIRE(fix2ref);
	REQUIRE(fix3ref);
	REQUIRE(fix4ref);
	// coverting the fixrefs
	MockedFix* fix1mockRef = dynamic_cast<MockedFix*>(fix1ref.get());
	MockedFix* fix2mockRef = dynamic_cast<MockedFix*>(fix2ref.get());
	MockedFix* fix3mockRef = dynamic_cast<MockedFix*>(fix3ref.get());
	MockedFix* fix4mockRef = dynamic_cast<MockedFix*>(fix4ref.get());
	// checking the mockedfixrefs
	REQUIRE(fix1mockRef);
	REQUIRE(fix2mockRef);
	REQUIRE(fix3mockRef);
	REQUIRE(fix4mockRef);
	// calling the differnt functions various times.
	for (int i = 0; i < nInit; i++)
		fixListRef->init();
	for (int i = 0; i < nSetup; i++)
		fixListRef->setup();
	for (int i = 0; i < nInitInt; i++)
		fixListRef->initial_integrate();
	for (int i = 0; i < nPreForce; i++)
		fixListRef->pre_force();
	for (int i = 0; i < nPostForce; i++)
		fixListRef->post_force();
	for (int i = 0; i < nFinInt; i++)
		fixListRef->final_integrate();
	// checking the output
	REQUIRE(fix1mockRef->nInit == nInit);
	REQUIRE(fix2mockRef->nInit == nInit);
	REQUIRE(fix3mockRef->nInit == nInit);
	REQUIRE(fix4mockRef->nInit == nInit);
	REQUIRE(fix1mockRef->nSetup == nSetup);
	REQUIRE(fix2mockRef->nSetup == nSetup);
	REQUIRE(fix3mockRef->nSetup == nSetup);
	REQUIRE(fix4mockRef->nSetup == nSetup);
	REQUIRE(fix1mockRef->nTimes == nInitInt);
	REQUIRE(fix2mockRef->nTimes == nPreForce);
	REQUIRE(fix3mockRef->nTimes == nPostForce);
	REQUIRE(fix4mockRef->nTimes == nFinInt);


}

