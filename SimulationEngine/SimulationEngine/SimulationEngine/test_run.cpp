#include "catch_amalgamated.hpp"
#include "test.hpp"

// unittest for the run command
// The job of the run is to orchestrate runs and other components of the system.
// here we are testing 
// if the run has called the correct functions of fixes, neighbor, integrator and forcefields
TEST_CASE("testing the run class")
{
	std::cout << "testing the run class" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// constant parameters
	constexpr int nSteps = 10;
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
	// creating the fix list
	std::unique_ptr<FixList> fixList = std::make_unique<FixList>(engine);
	// registering those fixes to the fixList
	fixList->addFix(std::move(fix1));;
	// regisetering the for list
	engine.setItem(std::move(fixList));
	// creating the mockedIntegrator
	std::unique_ptr<Integrator> integrator = std::make_unique<MockedIntegrator2>(engine);
	// registering that mocked integrator
	engine.setItem(std::move(integrator));
	// creating the mockedNeighbor
	std::unique_ptr<Neighbor> neighbor = std::make_unique<MockedNeighbor2>(engine);
	// registering the neighborlist
	engine.setItem(std::move(neighbor));
	// creating the mocked forcefield
	std::unique_ptr<ForceField> forcefield = std::make_unique<MockedForceField2>(engine);
	// registering the forcefield
	engine.setItem(std::move(forcefield));
	// creating the run object 
	std::unique_ptr<Run> run = std::make_unique<Run>(engine);
	// registering that run into the engine
	engine.setItem(std::move(run));
	// returning the fixList and mocked fixes
	auto& fixListRef = engine.getFixList();
	auto& fix1ref = fixListRef->returnFixById("1");
	// checking if they are present
	REQUIRE(fixListRef);
	REQUIRE(fix1ref);
	// coverting the fixrefs
	MockedFix* fix1mockRef = dynamic_cast<MockedFix*>(fix1ref.get());
	// checking the mockedfixrefs
	REQUIRE(fix1mockRef);
	// returning the integrator
	auto& integratorRef = engine.getIntegrator();
	// checking if it exits
	REQUIRE(integratorRef);
	// coverting it
	MockedIntegrator2* integratormockRef = dynamic_cast<MockedIntegrator2*>(integratorRef.get());
	// checking the conversion
	REQUIRE(integratormockRef);
	// returning the forcefield
	auto& forcefieldRef = engine.getForceField();
	// checking if it exits
	REQUIRE(forcefieldRef);
	// converting it
	MockedForceField2* forcefieldmockRef = dynamic_cast<MockedForceField2*>(forcefieldRef.get());
	// checking the conversion
	REQUIRE(forcefieldmockRef);
	// returning the neighbor
	auto& neighborRef = engine.getNeighbor();
	// checking if it exits
	REQUIRE(neighborRef);
	// converting it 
	MockedNeighbor2* neighbormockRef = dynamic_cast<MockedNeighbor2*>(neighborRef.get());
	// checking the conversion
	REQUIRE(neighbormockRef);
	// returning the run object
	auto& runRef = engine.getRunCommand();
	// checking if it exits
	REQUIRE(runRef);
	// running for 10 steps (previously the unittest of the fixlist has verified that fixs
	// are called in the right time)
	runRef->start(nSteps, 0);
	// init
	REQUIRE(fix1mockRef->nInit == 1);
	REQUIRE(forcefieldmockRef->nInit == 1);
	REQUIRE(neighbormockRef->nInit == 1);
	// setup
	REQUIRE(fix1mockRef->nSetup == 1);
	// update
	REQUIRE(forcefieldmockRef->nUpdates == nSteps);
	REQUIRE(neighbormockRef->nUpdates == nSteps);
	// nTimes
	REQUIRE(fix1mockRef->nTimes == nSteps);
	REQUIRE(integratormockRef->nTimes == nSteps);
}