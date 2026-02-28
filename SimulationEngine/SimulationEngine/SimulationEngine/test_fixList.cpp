#include "catch_amalgamated.hpp"
#include "test.hpp"

// unittest for the fixList class which orchestrate the fixes
// The main job of the fixList is to run each fix at designated time in the timestep
// It also keeps and returns a reference to fixes.
// Here we are testing 
// (1) putting fixes
// (2) getting fixes
// (3) testing running fixes in the correct time in each step
TEST_CASE("testing the fixList class")
{
	std::cout << "testing the fixList class" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// constant parameters
	constexpr int nInit = 8;
	constexpr int nSetup = 5;
	constexpr int nInitInt = 10;
	constexpr int nPreForce = 7;
	constexpr int nPostForce = 13;
	constexpr int nFinInt = 3;
	// building the mockedFixes
	std::unique_ptr<Fix> fix1 = std::make_unique<MockedFix>(FixMask::INIT_INTEGRATE, "1");
	std::unique_ptr<Fix> fix2 = std::make_unique<MockedFix>(FixMask::PRE_FORCE, "2");
	std::unique_ptr<Fix> fix3 = std::make_unique<MockedFix>(FixMask::POST_FORCE, "3");
	std::unique_ptr<Fix> fix4 = std::make_unique<MockedFix>(FixMask::FINAL_INTEGRATE, "4");
	// creating the integrator
	std::unique_ptr<FixList> fixList = std::make_unique<FixList>();
	// addting to the fixList
	fixList->addFix(std::move(fix1));
	fixList->addFix(std::move(fix2));
	fixList->addFix(std::move(fix3));
	fixList->addFix(std::move(fix4));
	// returning the fixList and mocked fixes
	auto& fix1ref = fixList->returnFixById("1");
	auto& fix2ref = fixList->returnFixById("2");
	auto& fix3ref = fixList->returnFixById("3");
	auto& fix4ref = fixList->returnFixById("4");
	// checking if they are present
	REQUIRE(fixList);
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
		fixList->init();
	for (int i = 0; i < nSetup; i++)
		fixList->setup();
	for (int i = 0; i < nInitInt; i++)
		fixList->initial_integrate();
	for (int i = 0; i < nPreForce; i++)
		fixList->pre_force();
	for (int i = 0; i < nPostForce; i++)
		fixList->post_force();
	for (int i = 0; i < nFinInt; i++)
		fixList->final_integrate();
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