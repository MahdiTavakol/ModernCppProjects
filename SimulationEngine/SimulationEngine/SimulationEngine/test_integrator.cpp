#include "catch_amalgamated.hpp"
#include "test.hpp"
#include <iostream>


class Test_integrator_fixture {
public:
	Test_integrator_fixture(
		double & dt_,
		std::unique_ptr<Integrator>& integrator_,
		std::vector<double>& x_,
		std::vector<double>& v_,
		std::vector<double>& f_,
		std::vector<double>& r_,
		std::vector<double>& m_):
		dt{dt_},
		integrator{std::move(integrator_)},
		x{std::move(x_)},
		v{std::move(v_)},
		f{std::move(f_)},
		r{std::move(r_)},
		m{std::move(m_)}
	{}
	void setup()
	{
		// number of particles
		int nlocal = x.size();
		int nmax = x.size();
		// particles
		mockedParticles = std::make_unique<MockedParticles>(nmax, x, v, f, r, m);
		// the timestep
		constexpr double dt = 1.0;
		// setting the timestep
		integrator->setDt(dt);
		// registering Particles and Integrator objects
		engine.setItem(std::move(integrator));
		engine.setItem(std::move(mockedParticles));
		// injecting dependencies
		engine.injectDependencies();
	}
	std::unique_ptr<Integrator>& return_integrator()
	{
		// returning the integrator object
		auto& integratorRef = engine.getIntegrator();
		return integratorRef;
	}
	std::unique_ptr<Particles>& return_particles()
	{
		auto& particlesRef = engine.getParticlesForUpdate();
		return particlesRef;
	}

private:
	double dt;
	Engine engine;
	std::unique_ptr<Particles> mockedParticles;
	std::unique_ptr<Integrator> integrator;
	std::vector<double> x, v, f, r, m;
};

// shared variables
constexpr int nmax = 5;
constexpr int nlocal = 5;
double dt = 1.0;

// integrator unittest
TEST_CASE("Testing the integrator class update positions") {
	std::cout << "Testing the integrator class update positions" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// put some particles.
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
	// the integrator object
	std::unique_ptr<Integrator> integrator =
		std::make_unique<MockedIntegrator>();
	// creating the test_integrator_fixture
	Test_integrator_fixture test_integrator_fixture(dt,integrator, x, v, f, r, m);
	// setting up the integration fixture
	test_integrator_fixture.setup();
	// returning the integrator ref from the engine
	auto& integratorRef = test_integrator_fixture.return_integrator();
	// testing the integratorRef
	REQUIRE(integratorRef);
	// converting it 
	MockedIntegrator* integratorRaw = dynamic_cast<MockedIntegrator*>(integratorRef.get());
	// checking the conversion
	REQUIRE(integratorRaw);
	// update xs
	integratorRaw->updateX();
	// returning the particlesRef
	auto& particlesRef = test_integrator_fixture.return_particles();
	// extracting the results
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
	// put some particles
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
	// the integrator object
	std::unique_ptr<Integrator> integrator =
		std::make_unique<MockedIntegrator>();
	// the test_integration_fixture
	Test_integrator_fixture test_integration_fixture(dt, integrator, x, v, f, r, m);
	// setting it up
	test_integration_fixture.setup();
	// calling the update_positions function of the integrator
	auto& integratorRef = test_integration_fixture.return_integrator();
	REQUIRE(integratorRef);
	// converting it
	MockedIntegrator* integratorRaw = dynamic_cast<MockedIntegrator*>(integratorRef.get());
	REQUIRE(integratorRaw);
	// updating the positions
	integratorRaw->updateX();
	// extracting the particles
	auto& particlesRef = test_integration_fixture.return_particles();
	// extracting the results
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

TEST_CASE("Testing the integrator class update velocities and positions at the same time")
{
	std::cout << "Testing the integrator class update velocities and positions at the same time" << std::endl;
	std::cout << std::string(80, '=');
	// put some particles
	std::vector<double> x = {
	   100.0,30.0,40.0,    // particle 0
	  -150.0,200.0,-10.0,  // particle 1
	   5.0,7.5,200.95,     // particle 2
	   0.0,-349.5,1000.54, // particle 3
	   230.5,0.95,145.78,  // particle 4
	};
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
	std::vector<double> m = { 20,30,40,10,10 };
	// the integrator object
	std::unique_ptr<Integrator> integrator =
		std::make_unique<MockedIntegrator>();
	// test_integrator_fixture
	Test_integrator_fixture test_integrator_fixture(dt,integrator ,x, v, f, r, m);
	// setting it up
	test_integrator_fixture.setup();
	// updating the positions
	auto& integratorRef = test_integrator_fixture.return_integrator();
	// checking the returned integrator
	REQUIRE(integratorRef);
	// converting it 
	MockedIntegrator* integratorRaw = dynamic_cast<MockedIntegrator*>(integratorRef.get());
	// checking the conversion
	REQUIRE(integratorRaw);
	// updating the positions
	integratorRaw->updateX();
	// updating the velocities
	integratorRaw->updateV();
	// getting the particles
	auto& particlesRef = test_integrator_fixture.return_particles();
	// checking it 
	REQUIRE(particlesRef);
	// getting Xs, Vs and fs
	double* Xs = particlesRef->getXData();
	double* Vs = particlesRef->getVData();
	double* Fs = particlesRef->getFData();
	// checking the pointers
	REQUIRE(Xs);
	REQUIRE(Vs);
	REQUIRE(Fs);
	// expected results
	std::vector<double> expectedXs = {
		142.73, -58.19, 55.64,   // Particle 0
	   -223.54, 294.07,-46.28, // Particle 1
		 16.95, -52.33,279.39,   // Particle 2
		-98.61,-326.33,955.62, // Particle 3
		297.58, -11.59,245.09   // Particle 4
	};
	std::vector<double> expectedVs = {
		42.916,-88.5975,15.737,  // Particle 0
		-73.736,94.371,-36.369,  // Particle 1
		11.9645,-59.95275,78.624,// Particle 2
		-99.537,23.384,-45.503,  // Particle 3
		67.941,-12.672,99.788    // Particle 4
	};
	std::vector<double> expectedFs = {
      3.72,  -8.15,   1.94,   // particle 0
	 -6.48,   9.03,  -2.67,   // particle 1
	  0.58,  -4.91,   7.36,   // particle 2
	 -9.27,   2.14,  -5.83,   // particle 3
	  8.61,  -1.32,   4.75,   // particle 4
	};
	// checking the results
	int nData = x.size();
	for (int i = 0; i < nData; i++) {
		REQUIRE_THAT(Xs[i], Catch::Matchers::WithinAbs(expectedXs[i], 1e-6));
		REQUIRE_THAT(Vs[i], Catch::Matchers::WithinAbs(expectedVs[i], 1e-6));
		REQUIRE_THAT(Fs[i], Catch::Matchers::WithinAbs(expectedFs[i], 1e-6));
	}



}

