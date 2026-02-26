#include "catch_amalgamated.hpp"
#include "test.hpp"


// unittest for the factory class
TEST_CASE("Testing the engine factory class to build the engine from commands") {
	std::cout << "Testing the engine factory class to build the engine from commands" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// the commands vector to build the engine
	std::vector<std::string> commands = {
		"box 0.0 0.0 0.0 100000.0 100000.0 100000.0",
		"particles 10",
		"particle 1 0.0 0.0 0.0 2.0 1.0 5.0 0.0 0.0 0.0 5.0",
		"particle 2 3.0 100.0 1000.0 8.0 -0.1 -10.0 0.0 0.0 0.0 5.0",
		"particle 3 700.0 20.0 50.0 -5.0 0.1 7.0 0.0 0.0 0.0 5.0",
		"integrator semi 1",
		"error screen",
		"fix print 1 1 init_integrate x 0",
		"fix print 2 1 init_integrate v 0",
		"forcefield spring 10.0",
		"neighbor simple 90.0",
		"run_status verbose",
		"run 100"
	};
	// building the engine factory
	EngineFixture enginefixture(commands);
	// building the engine using the fixture
	auto engine = enginefixture.returnEngine();
	// checking if the engine was built successfully

	// 1 - checking the box dimensions
	auto& boxRef = engine->getBox();
	REQUIRE(boxRef);
	std::array<double, 3> expectedMin = { 0.0,0.0,0.0 };
	std::array<double, 3> expectedMax = { 100000.0,100000.0,100000.0 };
	std::array<double, 3> actualMin, actualMax;
	boxRef->getDimensions(actualMin, actualMax);
	for (int i = 0; i < 3; i++) {
		REQUIRE_THAT(expectedMin[i], Catch::Matchers::WithinAbs(actualMin[i], 1e-6));
		REQUIRE_THAT(expectedMax[i], Catch::Matchers::WithinAbs(actualMax[i], 1e-6));
	}
	// 2 - checking the max particles
	auto& particlesRef = engine->getParticles();
	REQUIRE(particlesRef);
	int nmax, nlocal;
	particlesRef->getNmaxNlocal(nmax, nlocal);
	REQUIRE(nmax == 10);

	// 3 - checking the integrator type
	Integrator* integratorRaw = engine->getIntegrator().get();
	REQUIRE(integratorRaw);
	if (auto semiIntegratorPtr = dynamic_cast<SemiIntegrator*>(integratorRaw)) {
		SUCCEED("The integrator is of type SemiIntegrator");
	}
	else {
		FAIL("The integrator is not of type SemiIntegrator");
	}

	// 4 - checking the error streams
	auto& errorRef = engine->getError();
	REQUIRE(errorRef);
	auto& errorStreams = errorRef->getStreams();
	REQUIRE(errorStreams.size() == 1);
	if (&errorStreams[0].get() == &std::cout) {
		SUCCEED("The error stream is set to std::cout");
	}
	else {
		FAIL("The error stream is not set to std::cout");
	}

	// 5 - checking the fix prints
	auto& fix1Ref = engine->returnFixById("1");
	auto& fix2Ref = engine->returnFixById("2");
	REQUIRE(fix1Ref);
	REQUIRE(fix2Ref);
	if (auto fixPtr = dynamic_cast<FixPrint*>(fix1Ref.get())
		&& dynamic_cast<FixPrint*>(fix2Ref.get())) {
		SUCCEED("The fixs with ids print1 and print2 are of type print");
	}
	else {
		FAIL("The fixs with ids print1 and print2 are not of type print");
	}

	// 6 - checking the force field
	auto& forcefield = engine->getForceField();
	REQUIRE(forcefield);
	if (auto springFieldPtr = dynamic_cast<SpringField*>(forcefield.get())) {
		SUCCEED("The force field is of type SpringField");
	}
	else {
		FAIL("The force field is not of type SpringField");
	}

	// 7 - checking the neighbor
	auto& neighbor = engine->getNeighbor();
	REQUIRE(neighbor);
	double expectedCutoff = 90.0;
	double actualCutoff = neighbor->getCutoff();
	REQUIRE(expectedCutoff == actualCutoff);
	if (auto simpleNeighborPtr = dynamic_cast<SimpleNeighbor*>(neighbor.get())) {
		SUCCEED("The neighbor is of type SimpleNeighbor");
	}
	else {
		FAIL("The neighbor is not of type SimpleNeighbor");
	}

	// 8 - checking the run status
	auto& runStatus = engine->getStatus();
	REQUIRE(runStatus == Engine::Run_Status::VERBOSE);

	// 9 - checking the run command
	auto& runCommand = engine->getRunCommand();
	REQUIRE(runCommand);
	int expectedSteps = 100;
	int actualSteps = runCommand->getSteps();
	REQUIRE(expectedSteps == actualSteps);

	// 10 - checking the particles
	std::vector<std::array<double, 3>> expectedXs = {
	{ 0.0,0.0,0.0 },
	{ 3.0,100.0,1000.0 },
	{ 700.0,20.0,50.0 }
	};
	std::vector<std::array<double, 3>> expectedVs = {
	{ 2.0,1.0,5.0 },
	{ 8.0,-0.1,-10.0 },
	{ -5.0,0.1,7.0 }
	};
	std::vector<std::array<double, 3>> expectedFs = {
	{ 0.0,0.0,0.0 },
	{ 0.0,0.0,0.0 },
	{ 0.0,0.0,0.0 }
	};
	auto& particlesForUpdate = engine->getParticlesForUpdate();
	for (int i = 0; i < 3; i++) {
		std::array<double, 3> actualX, actualV, actualF;
		double actualM;
		particlesForUpdate->getAtomVec(i, actualX, actualV, actualF, actualM);
		for (int j = 0; j < 3; j++) {
			REQUIRE_THAT(expectedXs[i][j], Catch::Matchers::WithinAbs(actualX[j], 1e-6));
			REQUIRE_THAT(expectedVs[i][j], Catch::Matchers::WithinAbs(actualV[j], 1e-6));
			REQUIRE_THAT(expectedFs[i][j], Catch::Matchers::WithinAbs(actualF[j], 1e-6));
		}
		REQUIRE_THAT(5.0, Catch::Matchers::WithinAbs(actualM, 1e-6));
	}
}