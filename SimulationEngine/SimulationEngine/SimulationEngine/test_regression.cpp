#include <iostream>
#include <array>

#include "catch_amalgamated.hpp"
#include "test.hpp"

TEST_CASE("Starting and registering each class of the Engine class with minimal input args")
{
	std::cout << "Testing the starting and registering of each class of the Engine class with minimal input args" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// constant variables
	int nmax = 10;
	array<double, 3> min = { 0.0,0.0,0.0 };
	array<double, 3> max = { 10.0,10.0,10.0 };
	// the mocked Engine object
	Engine mockedEngine;
	// box 
	auto box = make_unique<Box>(min, max);
	// particles
	auto particles = make_unique<Particles>(nmax);
	// injecting dependencies
	box->injectDependencies(mockedEngine);
	particles->injectDependencies(mockedEngine);
	// parameters before registering into the engine
	array<double, 3> rMin1, rMax1, rMin2, rMax2;
	int rNmax1, rNlocal1, rNmax2, rNlocal2;
	box->getDimensions(rMin1, rMax1);
	particles->getNmaxNlocal(rNmax1, rNlocal1);
	// adding these to the engine
	mockedEngine.setItem(std::move(box));
	mockedEngine.setItem(std::move(particles));
	// parameters afer registering into the engine
	Ref* boxPtrBase = mockedEngine.getItem(Engine::ItemRef::BOX);
	auto boxPtr = dynamic_cast<Box*>(boxPtrBase);
	const unique_ptr<Particles>& particlesE = mockedEngine.getParticles();
	boxPtr->getDimensions(rMin2, rMax2);
	particlesE->getNmaxNlocal(rNmax2, rNlocal2);
	// checking if they are equal
	REQUIRE(rMin1 == rMin2);
	REQUIRE(rMax1 == rMax2);
	REQUIRE(rNmax1 == rNmax2);
	REQUIRE(rNlocal1 == rNlocal2);

}

TEST_CASE("Testing the Euler integration in 100 steps for 3 particles under constant velocity condition")
{
	std::cout << "Testing the Euler integration in 100 steps for 3 particles under constant velocity condition" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	constexpr int nSteps = 100;
	// constant variables
	int nmax = 10;
	array<double, 3> min = { 0.0,0.0,0.0 };
	array<double, 3> max = { 1000.0,1000.0,1000.0 };
	// the mocked Engine object
	Engine mockedEngine;
	// box 
	auto box = make_unique<Box>(min, max);
	// particles
	auto particles = make_unique<Particles>(nmax);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<EulerIntegrator>();
	// mocked forcefield
	//unique_ptr<ForceField> forcefield = make_unique<MockedForceField>();
	// mocked neighbor
	//unique_ptr<Neighbor> neighbor = make_unique<MockedNeighbor>();

	// creating the run object
	auto run = make_unique<Run>();
	// adding these to the engine
	mockedEngine.setItem(std::move(box));
	mockedEngine.setItem(std::move(particles));
	mockedEngine.setItem(std::move(integrator));
	mockedEngine.setItem(std::move(run));
	// injecting dependencies
	mockedEngine.injectDependencies();
	// getting a reference to particles
	auto& engineParticles = mockedEngine.getParticlesForUpdate();
	// new particles
	vector<array<double, 3>> newXs, newVs, newFs;
	vector<double> newMs;
	// particle #1
	newXs.push_back({ 0.0,0.0,0.0 });
	newVs.push_back({ 2.0,1.0,5.0 });
	newFs.push_back({ 0.0,0.0,0.0 });
	newMs.push_back(5.0);
	// particle #2
	newXs.push_back({ 3.0,100.0,1000.0 });
	newVs.push_back({ 8.0,-0.1,-10.0 });
	newFs.push_back({ 0.0,0.0,0.0 });
	newMs.push_back(5.0);
	// particle #3
	newXs.push_back({ 700.0,20.0,50.0 });
	newVs.push_back({ -5.0,0.1,7.0 });
	newFs.push_back({ 0.0,0.0,0.0 });
	newMs.push_back(5.0);
	// expected values
	vector<array<double, 3>> finalXs, finalVs, finalFs;
	vector<double> finalMs;
	// particle #1
	finalXs.push_back({ 200.0,100.0,500.0 });
	finalVs.push_back({ 2.0,1.0,5.0 });
	finalFs.push_back({ 0.0,0.0,0.0 });
	finalMs.push_back(5.0);
	// particle #2
	finalXs.push_back({ 803.0,90.0,0.0 });
	finalVs.push_back({ 8.0,-0.1,-10.0 });
	finalFs.push_back({ 0.0,0.0,0.0 });
	finalMs.push_back(5.0);
	// particle #3
	finalXs.push_back({ 200.0,30.0,750.0 });
	finalVs.push_back({ -5.0,0.1,7.0 });
	finalFs.push_back({ 0.0,0.0,0.0 });
	finalMs.push_back(5.0);

	// adding it 
	for (int i = 0; i < 3; i++) {
		engineParticles->addParticle(newXs[i], newVs[i], newFs[i], newMs[i]);
	}
	// getting the run command
	auto& runCommand = mockedEngine.getRunCommand();
	// setting it up
	runCommand->setup();
	// running it for 100 steps
	runCommand->start(nSteps);
	// comparing the results
	for (int i = 0; i < 3; i++) {
		array<double, 3> xi, vi, fi;
		double mi;
		engineParticles->getAtomVec(i, xi, vi, fi, mi);
		for (int j = 0; j < 3; j++) {
			REQUIRE_THAT(xi[j], Catch::Matchers::WithinAbs(finalXs[i][j], 1e-6));
			REQUIRE_THAT(vi[j], Catch::Matchers::WithinAbs(finalVs[i][j], 1e-6));
			REQUIRE_THAT(fi[j], Catch::Matchers::WithinAbs(finalFs[i][j], 1e-6));
		}
		REQUIRE_THAT(mi, Catch::Matchers::WithinAbs(finalMs[i], 1e-6));
	}
}

TEST_CASE("Testing the Euler integration in 100 steps for 3 particles under constant acceleration condition")
{
	std::cout << "Testing the Euler integration in 100 steps for 3 particles under constant acceleration condition" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	constexpr int nSteps = 100;
	// constant variables
	int nmax = 10;
	array<double, 3> min = { -100000.0,-100000.0,-100000.0 };
	array<double, 3> max = { 100000.0,100000.0,100000.0 };
	// the mocked Engine object
	Engine mockedEngine;
	// box 
	auto box = make_unique<Box>(min, max);
	// particles
	auto particles = make_unique<Particles>( nmax);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<EulerIntegrator>();
	// adding these to the engine
	mockedEngine.setItem(std::move(box));
	mockedEngine.setItem(std::move(particles));
	mockedEngine.setItem(std::move(integrator));
	// getting a reference to particles
	auto& engineParticles = mockedEngine.getParticlesForUpdate();
	// new particles
	vector<array<double, 3>> newXs, newVs, newFs;
	vector<double> newMs;
	// particle #1
	newXs.push_back({ 0.0,0.0,0.0 });
	newVs.push_back({ 2.0,1.0,5.0 });
	newFs.push_back({ 5.0,8.0,-10.0 });
	newMs.push_back(5.0);
	// particle #2
	newXs.push_back({ 3.0,100.0,1000.0 });
	newVs.push_back({ 8.0,-0.1,-10.0 });
	newFs.push_back({ 0.1,2.0,-10.0 });
	newMs.push_back(5.0);
	// particle #3
	newXs.push_back({ 700.0,20.0,50.0 });
	newVs.push_back({ -5.0,0.1,7.0 });
	newFs.push_back({ 0.1,3.0,0.2 });
	newMs.push_back(5.0);
	// expected values
	vector<array<double, 3>> finalXs, finalVs, finalFs;
	vector<double> finalMs;
	// particle #1
	finalXs.push_back({ 5150.0,8020.0,-9400.0 });
	finalVs.push_back({ 102.0,161.0,-195.0 });
	finalFs.push_back({ 5.0,8.0,-10.0 });
	finalMs.push_back(5.0);
	// particle #2
	finalXs.push_back({ 902.0,2070.0,-9900.0 });
	finalVs.push_back({ 10.0,39.9,-210.0 });
	finalFs.push_back({ 0.1,2.0,-10.0 });
	finalMs.push_back(5.0);
	// particle #3
	finalXs.push_back({ 299.0,3000.0,948.0 });
	finalVs.push_back({ -3.0,60.1,11.0 });
	finalFs.push_back({ 0.1,3.0,0.2 });
	finalMs.push_back(5.0);

	// adding it 
	for (int i = 0; i < 3; i++) {
		engineParticles->addParticle(newXs[i], newVs[i], newFs[i], newMs[i]);
	}
	// injecting dependencies
	mockedEngine.injectDependencies();
	// creating the run object
	auto run = make_unique<Run>();
	// injecting dependencies
	run->injectDependencies(mockedEngine);
	// setting it up
	run->setup();
	// running it for 100 steps
	run->start(nSteps);
	// comparing the results
	for (int i = 0; i < 3; i++) {
		array<double, 3> xi, vi, fi;
		double mi;
		engineParticles->getAtomVec(i, xi, vi, fi, mi);
		for (int j = 0; j < 3; j++) {
			REQUIRE_THAT(xi[j], Catch::Matchers::WithinAbs(finalXs[i][j], 1e-6));
			REQUIRE_THAT(vi[j], Catch::Matchers::WithinAbs(finalVs[i][j], 1e-6));
			REQUIRE_THAT(fi[j], Catch::Matchers::WithinAbs(finalFs[i][j], 1e-6));
		}
		REQUIRE_THAT(mi, Catch::Matchers::WithinAbs(finalMs[i], 1e-6));
	}
}


TEST_CASE("Testing the Semi-Euler integration in 100 steps for 3 particles under constant velocity condition")
{
	std::cout << "Testing the Semi-Euler integration in 100 steps for 3 particles under constant velocity condition" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	constexpr int nSteps = 100;
	// constant variables
	int nmax = 10;
	array<double, 3> min = { 0.0,0.0,0.0 };
	array<double, 3> max = { 1000.0,1000.0,1000.0 };
	// the mocked Engine object
	Engine mockedEngine;
	// box 
	auto box = make_unique<Box>(min, max);
	// particles
	auto particles = make_unique<Particles>(nmax);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<SemiIntegrator>();
	// adding these to the engine
	mockedEngine.setItem(std::move(box));
	mockedEngine.setItem(std::move(particles));
	mockedEngine.setItem(std::move(integrator));
	// getting a reference to particles
	auto& engineParticles = mockedEngine.getParticlesForUpdate();
	// new particles
	vector<array<double, 3>> newXs, newVs, newFs;
	vector<double> newMs;
	// particle #1
	newXs.push_back({ 0.0,0.0,0.0 });
	newVs.push_back({ 2.0,1.0,5.0 });
	newFs.push_back({ 0.0,0.0,0.0 });
	newMs.push_back(5.0);
	// particle #2
	newXs.push_back({ 3.0,100.0,1000.0 });
	newVs.push_back({ 8.0,-0.1,-10.0 });
	newFs.push_back({ 0.0,0.0,0.0 });
	newMs.push_back(5.0);
	// particle #3
	newXs.push_back({ 700.0,20.0,50.0 });
	newVs.push_back({ -5.0,0.1,7.0 });
	newFs.push_back({ 0.0,0.0,0.0 });
	newMs.push_back(5.0);
	// expected values
	vector<array<double, 3>> finalXs, finalVs, finalFs;
	vector<double> finalMs;
	// particle #1
	finalXs.push_back({ 200.0,100.0,500.0 });
	finalVs.push_back({ 2.0,1.0,5.0 });
	finalFs.push_back({ 0.0,0.0,0.0 });
	finalMs.push_back(5.0);
	// particle #2
	finalXs.push_back({ 803.0,90.0,0.0 });
	finalVs.push_back({ 8.0,-0.1,-10.0 });
	finalFs.push_back({ 0.0,0.0,0.0 });
	finalMs.push_back(5.0);
	// particle #3
	finalXs.push_back({ 200.0,30.0,750.0 });
	finalVs.push_back({ -5.0,0.1,7.0 });
	finalFs.push_back({ 0.0,0.0,0.0 });
	finalMs.push_back(5.0);

	// adding it 
	for (int i = 0; i < 3; i++) {
		engineParticles->addParticle(newXs[i], newVs[i], newFs[i], newMs[i]);
	}
	// injecting dependencies
	mockedEngine.injectDependencies();
	// creating the run object
	auto run = make_unique<Run>();
	// injecting dependencies
	run->injectDependencies(mockedEngine);
	// setting it up
	run->setup();
	// running it for 100 steps
	run->start(nSteps);
	// comparing the results
	for (int i = 0; i < 3; i++) {
		array<double, 3> xi, vi, fi;
		double mi;
		engineParticles->getAtomVec(i, xi, vi, fi, mi);
		for (int j = 0; j < 3; j++) {
			REQUIRE_THAT(xi[j], Catch::Matchers::WithinAbs(finalXs[i][j], 1e-6));
			REQUIRE_THAT(vi[j], Catch::Matchers::WithinAbs(finalVs[i][j], 1e-6));
			REQUIRE_THAT(fi[j], Catch::Matchers::WithinAbs(finalFs[i][j], 1e-6));
		}
		REQUIRE_THAT(mi, Catch::Matchers::WithinAbs(finalMs[i], 1e-6));
	}
}

TEST_CASE("Testing the Semi-Euler integration in 100 steps for 3 particles under constant acceleration condition")
{
	std::cout << "Testing the Semi-Euler integration in 100 steps for 3 particles under constant acceleration condition" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	constexpr int nSteps = 100;
	// constant variables
	int nmax = 10;
	array<double, 3> min = { -100000.0,-100000.0,-100000.0 };
	array<double, 3> max = { 100000.0,100000.0,100000.0 };
	// the mocked Engine object
	Engine mockedEngine;
	// box 
	auto box = make_unique<Box>(min, max);
	// particles
	auto particles = make_unique<Particles>(nmax);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<SemiIntegrator>();
	// adding these to the engine
	mockedEngine.setItem(std::move(box));
	mockedEngine.setItem(std::move(particles));
	mockedEngine.setItem(std::move(integrator));
	// getting a reference to particles
	auto& engineParticles = mockedEngine.getParticlesForUpdate();
	// new particles
	vector<array<double, 3>> newXs, newVs, newFs;
	vector<double> newMs;
	// particle #1
	newXs.push_back({ 0.0,0.0,0.0 });
	newVs.push_back({ 2.0,1.0,5.0 });
	newFs.push_back({ 5.0,8.0,-10.0 });
	newMs.push_back(5.0);
	// particle #2
	newXs.push_back({ 3.0,100.0,1000.0 });
	newVs.push_back({ 8.0,-0.1,-10.0 });
	newFs.push_back({ 0.1,2.0,-10.0 });
	newMs.push_back(5.0);
	// particle #3
	newXs.push_back({ 700.0,20.0,50.0 });
	newVs.push_back({ -5.0,0.1,7.0 });
	newFs.push_back({ 0.1,3.0,0.2 });
	newMs.push_back(5.0);
	// expected values
	vector<array<double, 3>> finalXs, finalVs, finalFs;
	vector<double> finalMs;
	// particle #1
	finalXs.push_back({ 5250.0,8180.0,-9600.0 });
	finalVs.push_back({ 102.0,161.0,-195.0 });
	finalFs.push_back({ 5.0,8.0,-10.0 });
	finalMs.push_back(5.0);
	// particle #2
	finalXs.push_back({ 904.0,2110.0,-10100.0 });
	finalVs.push_back({ 10.0,39.9,-210.0 });
	finalFs.push_back({ 0.1,2.0,-10.0 });
	finalMs.push_back(5.0);
	// particle #3
	finalXs.push_back({ 301.0,3060.0,952.0 });
	finalVs.push_back({ -3.0,60.1,11.0 });
	finalFs.push_back({ 0.1,3.0,0.2 });
	finalMs.push_back(5.0);

	// adding it 
	for (int i = 0; i < 3; i++) {
		engineParticles->addParticle(newXs[i], newVs[i], newFs[i], newMs[i]);
	}
	// injecting dependencies
	mockedEngine.injectDependencies();
	// creating the run object
	auto run = make_unique<Run>();
	// injecting dependencies
	run->injectDependencies(mockedEngine);
	// setting it up
	run->setup();
	// running it for 100 steps
	run->start(nSteps);
	// comparing the results
	for (int i = 0; i < 3; i++) {
		array<double, 3> xi, vi, fi;
		double mi;
		engineParticles->getAtomVec(i, xi, vi, fi, mi);
		for (int j = 0; j < 3; j++) {
			REQUIRE_THAT(xi[j], Catch::Matchers::WithinAbs(finalXs[i][j], 1e-6));
			REQUIRE_THAT(vi[j], Catch::Matchers::WithinAbs(finalVs[i][j], 1e-6));
			REQUIRE_THAT(fi[j], Catch::Matchers::WithinAbs(finalFs[i][j], 1e-6));
		}
		REQUIRE_THAT(mi, Catch::Matchers::WithinAbs(finalMs[i], 1e-6));
	}
}

TEST_CASE("Testing fixes invoked at initial_integrate and final_integrate steps") {
	std::cout << "Testing fixes invoked at initial_integrate and final_integrate steps" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// number of steps
	constexpr int nSteps = 5;
	// the commands vector to build the engine
	std::vector<std::string> commands = {
		"box -100000.0 -100000.0 -100000.0 100000.0 100000.0 100000.0",
		"particles 10",
		"particle 1 0.0 0.0 0.0 2.0 1.0 5.0 5.0 8.0 -10.0 5.0",
		"integrator semi 1",
		"forcefield spring 10.0",
		"run_status silent",
		"fix print 1 1 init_integrate x 0",
		"fix print 2 1 init_integrate v 0",
		"fix print 3 1 init_integrate f 0",
		"fix print 4 1 final_integrate x 0",
		"fix print 5 1 final_integrate v 0",
		"fix print 6 1 final_integrate f 0",
		"run " + std::to_string(nSteps)
	};

	// building the engineFixture
	EngineFixture engineFixture{ commands };
	// running the engine
	engineFixture.runEngine();


	// expected values
	vector<array<double, 3>> expectedXs, expectedVs, expectedFs;

	// xs
	expectedXs.push_back({ 0.0,0.0,0.0 });
	expectedXs.push_back({ 3.0,2.6,3.0 });
	expectedXs.push_back({ 7.0,6.8,4.0 });
	expectedXs.push_back({ 12.0,12.6,3.0 });
	expectedXs.push_back({ 18.0,20.0,0.0 });
	expectedXs.push_back({ 25.0,29.0,-5.0 });
	// vs
	expectedVs.push_back({ 2.0,1.0,5.0 });
	expectedVs.push_back({ 3.0,2.6,3.0 });
	expectedVs.push_back({ 4.0,4.2,1.0 });
	expectedVs.push_back({ 5.0,5.8,-1.0 });
	expectedVs.push_back({ 6.0,7.4,-3.0 });
	expectedVs.push_back({ 7.0,9.0,-5.0 });
	// fs
	expectedFs.push_back({ 5.0,8.0,-10.0 });
	expectedFs.push_back({ 5.0,8.0,-10.0 });
	expectedFs.push_back({ 5.0,8.0,-10.0 });
	expectedFs.push_back({ 5.0,8.0,-10.0 });
	expectedFs.push_back({ 5.0,8.0,-10.0 });
	expectedFs.push_back({ 5.0,8.0,-10.0 });


	// getting the fixprints
	auto& fix1ref = engineFixture.returnFixById("1");
	auto& fix2ref = engineFixture.returnFixById("2");
	auto& fix3ref = engineFixture.returnFixById("3");
	auto& fix4ref = engineFixture.returnFixById("4");
	auto& fix5ref = engineFixture.returnFixById("5");
	auto& fix6ref = engineFixture.returnFixById("6");
	// casting the fixPrint
	FixPrint* const fixPrintPtr1 = dynamic_cast<FixPrint*>(fix1ref.get());
	FixPrint* const fixPrintPtr2 = dynamic_cast<FixPrint*>(fix2ref.get());
	FixPrint* const fixPrintPtr3 = dynamic_cast<FixPrint*>(fix3ref.get());
	FixPrint* const fixPrintPtr4 = dynamic_cast<FixPrint*>(fix4ref.get());
	FixPrint* const fixPrintPtr5 = dynamic_cast<FixPrint*>(fix5ref.get());
	FixPrint* const fixPrintPtr6 = dynamic_cast<FixPrint*>(fix6ref.get());
	// checking the Ref of the Fix
	REQUIRE(fixPrintPtr1);
	REQUIRE(fixPrintPtr2);
	REQUIRE(fixPrintPtr3);
	REQUIRE(fixPrintPtr4);
	REQUIRE(fixPrintPtr5);
	REQUIRE(fixPrintPtr6);
	// getting fixprint outputs;
	auto initXs = fixPrintPtr1->getOutputVector();
	auto initVs = fixPrintPtr2->getOutputVector();
	auto initFs = fixPrintPtr3->getOutputVector();
	auto finalXs = fixPrintPtr4->getOutputVector();
	auto finalVs = fixPrintPtr5->getOutputVector();
	auto finalFs = fixPrintPtr6->getOutputVector();

	// comparing the results
	for (int i = 0; i < nSteps; i++) {
		for (int j = 0; j < 3; j++) {
 			REQUIRE_THAT(expectedXs[i][j], Catch::Matchers::WithinAbs(initXs[i][j], 1e-6));
			REQUIRE_THAT(expectedVs[i][j], Catch::Matchers::WithinAbs(initVs[i][j], 1e-6));
			REQUIRE_THAT(expectedFs[i][j], Catch::Matchers::WithinAbs(initFs[i][j], 1e-6));
			REQUIRE_THAT(expectedXs[i + 1][j], Catch::Matchers::WithinAbs(finalXs[i][j], 1e-6));
			REQUIRE_THAT(expectedVs[i + 1][j], Catch::Matchers::WithinAbs(finalVs[i][j], 1e-6));
			REQUIRE_THAT(expectedFs[i + 1][j], Catch::Matchers::WithinAbs(finalFs[i][j], 1e-6));
		}
	}
}


TEST_CASE("Calculating the force from the forcefield equation") {
	std::cout << "Testing the calculation of the force from the forcefield equation" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// number of steps
	constexpr int nSteps = 5;
	// the commands vector to build the engine
	std::vector<std::string> commands = {
		"box -100000.0 -100000.0 -100000.0 100000.0 100000.0 100000.0",
		"particles 10",
		"particle 1 0.0 0.0 0.0 2.0 1.0 5.0 0.0 0.0 0.0 5.0",
		"particle 2 3.0 100.0 1000.0 8.0 -0.1 -10.0 0.0 0.0 0.0 5.0",
		"integrator semi 1",
		"forcefield spring 10.0",
		"run_status silent",
		"fix print 1 1 post_force f 0",
		"fix print 2 1 post_force f 1",
		"run " + std::to_string(nSteps)
	};
	// building the engine factory
	EngineFixture engineFixture{ commands };
	auto engine = engineFixture.returnEngine();
	// getting the run object 
	auto& run = engine->getRunCommand();
	// setting up the run
	run->setup();
	// starting the run
	run->start();
	// getting the fixprints
	auto& fix1ref = engine->returnFixById("1");
	auto& fix2ref = engine->returnFixById("2");

	// casting the fixPrint
	FixPrint* const fixPrintPtr1 = dynamic_cast<FixPrint*>(fix1ref.get());
	FixPrint* const fixPrintPtr2 = dynamic_cast<FixPrint*>(fix2ref.get());
	// checking the Ref of the Fix
	REQUIRE(fixPrintPtr1);
	REQUIRE(fixPrintPtr2);

	// getting fixprint outputs;
	auto F1s = fixPrintPtr1->getOutputVector();
	auto F2s = fixPrintPtr2->getOutputVector();


	// expected values
	vector<array<double, 3>> expectedF1s, expectedF2s;

	// Particle #1
	expectedF1s.push_back({ 30.0,1000.0,10000.0 });
	expectedF1s.push_back({ -30.0,-3011.0,-30150.0 });
	expectedF1s.push_back({ 30.0, 5022.0, 50300.0 });
	expectedF1s.push_back({ -30.0,-7033.0,-70450.0 });
	expectedF1s.push_back({ 30.0, 9044.0, 90600.0 });
	expectedF1s.push_back({ -30.0,-11055.0,-110750.0 });
	// Particle #2
	for (auto& f : expectedF1s)
		expectedF2s.push_back({ -f[0],-f[1],-f[2] });

	// comparing the results
	for (int i = 0; i < nSteps; i++) {
		std::cout << i << std::endl;
		for (int j = 0; j < 3; j++) {
			REQUIRE_THAT(expectedF1s[i][j], Catch::Matchers::WithinAbs(F1s[i][j], 1e-6));
			REQUIRE_THAT(expectedF2s[i][j], Catch::Matchers::WithinAbs(F2s[i][j], 1e-6));
		}
	}
}

TEST_CASE("Testing the silent and verbose version of the engine") {
	std::cout << "Testing the silent and verbose version of the engine" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// run_status
	struct {
		std::string messages;
		std::string status_string;
	} run_array[2] = {
		{"Running the silent version of the engine",
		 "silent"},
		{"Running the verbose version of the engine",
		 "verbose"},
	};


	// number of steps
	constexpr int nSteps = 5;
	std::string run_command = "run " + std::to_string(nSteps);

	for (auto& run : run_array) {
		std::cout << run.messages << std::endl;
		// the commands vector to build the engine
		std::vector<std::string> commands = {
			"box -100000.0 -100000.0 -100000.0 100000.0 100000.0 100000.0",
			"particles 10",
			"particle 1 0.0 0.0 0.0 2.0 1.0 5.0 0.0 0.0 0.0 5.0",
			"particle 2 3.0 100.0 1000.0 8.0 -0.1 -10.0 0.0 0.0 0.0 5.0",
			"integrator semi 1",
			"error screen",
			"run_status " + run.status_string,
			run_command
		};

		// building the engine factory
		EngineFactory factory(commands);
		// building the engine using the factory
		auto engine = factory.returnEngine();
		auto& run = engine->getRunCommand();
		// running the engine
		run->setup();
		run->start();
	}
}


TEST_CASE("Testing the neighbor list construction and updating") {
	std::cout << "Testing the neighbor list construction and updating" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// number of steps
	constexpr int nSteps = 5;
	std::string run_command = "run " + std::to_string(nSteps);
	// neighbor cutoff
	double neighbor_cutoff = 90.0;
	std::string neighbor_command = "neighbor simple " + std::to_string(neighbor_cutoff);
	// the commands vector to build the engine
	std::vector<std::string> commands = {
		"box -100000.0 -100000.0 -100000.0 100000.0 100000.0 100000.0",
		"particles 10",
		"particle 1 -24.0 -94.0 -37.0 0.0 0.0 0.0 0.0 0.0 0.0 5.0",
		"particle 2 -27.0 -20.0 -25.0 0.0 0.0 0.0 0.0 0.0 0.0 5.0",
		"particle 3 -49.0 98.0 96.0 0.0 0.0 0.0 0.0 0.0 0.0 5.0",
		"particle 4 13.0 -3.0 23.0 0.0 0.0 0.0 0.0 0.0 0.0 5.0",
		"particle 5 22.0 -27.0 89.0 0.0 0.0 0.0 0.0 0.0 0.0 5.0",
		"particle 6 97.0 19.0 80.0 0.0 0.0 0.0 0.0 0.0 0.0 5.0",
		"particle 7 -10.0 14.0 -23.0 0.0 0.0 0.0 0.0 0.0 0.0 5.0",
		"particle 8 -79.0 -89.0 -71.0 0.0 0.0 0.0 0.0 0.0 0.0 5.0",
		"particle 9 100.0 -58.0 -90.0 0.0 0.0 0.0 0.0 0.0 0.0 5.0",
		"particle 10 -86.0 32.0 85.0 0.0 0.0 0.0 0.0 0.0 0.0 5.0",
		"integrator semi 1",
		"error screen",
		neighbor_command,
		"run_status silent",
		run_command
	};
	// building the engine factory
	EngineFixture engineFixture{ commands };
	auto engine = engineFixture.returnEngine();
	auto& run = engine->getRunCommand();
	// running the engine
	run->setup();
	run->start();
	// expected neighbor list
	std::map<int, std::vector<int>> expectedNeighborList = {
	{ 1, { 2, 8 } },
	{ 2, {1, 4, 7} },
	{ 3, {10} },
	{ 4, {2, 5, 7} },
	{ 5, {4, 6} },
	{ 6, {5} },
	{ 7, {2, 4} },
	{ 8, {1} },
	{ 10,{3} } };
	// getting the neighbor list
	auto& neighborPtr = engine->getNeighbor();
	int nNeigh = 0;
	int* neighList = nullptr, * firstNeigh = nullptr, * numNeigh = nullptr;
	neighborPtr->getNeighborList(nNeigh, neighList, firstNeigh, numNeigh);
	// comparing the results
	for (int i = 0; i < nNeigh; i++) {
		int particleId = i + 1;
		std::vector<int> expectedNeighbors = expectedNeighborList[particleId];
		std::vector<int> actualNeighbors(neighList + firstNeigh[i], neighList + firstNeigh[i] + numNeigh[i]);
		for (int& neighborId : actualNeighbors) {
			neighborId += 1; // converting from 0-based to 1-based indexing
		}
		REQUIRE_THAT(actualNeighbors, Catch::Matchers::UnorderedEquals(expectedNeighbors));
	}
}


TEST_CASE("Testing collision integrator")
{
	std::cout << "Testing the collision integrator" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// number of steps
	constexpr int nSteps = 10;
	std::string run_command = "run " + std::to_string(nSteps);
	// spring coeff
	constexpr double spring_coeff = 0.95;
	std::string spring_command = "forcefield spring " + std::to_string(spring_coeff);
	// neighbor cutoff
	double neighbor_cutoff = 90.0;
	std::string neighbor_command = "neighbor simple " + std::to_string(neighbor_cutoff);
	// timestep
	double dt = 0.1;
	std::string integrator_command = "integrator collision " + std::to_string(dt);
	// the commands vector to build the engine
	std::vector<std::string> commands = {
		"box -100000.0 -100000.0 -100000.0 100000.0 100000.0 100000.0",
		"particles 10",
		"particle 1 0.0 0.0 0.0 2.0 1.0 5.0 0.0 0.0 0.0 5.0 radius 100",
		"particle 2 3.0 100 1000.0 8.0 -0.1 -10.0 0.0 0.0 0.0 10.0 radius 100",
		"fix print 1 1 init_integrate x 0",
		"fix print 2 1 init_integrate f 0",
		spring_command,
		integrator_command,
		"error screen",
		neighbor_command,
		"run_status silent",
		run_command
	};
	// building the engine factory
	EngineFixture enginefixture(commands);
	// building the engine using the fixture
	auto engine = enginefixture.returnEngine();
	// run
	auto& run = engine->getRunCommand();
	// running the engine
	run->setup();
	run->start();
	// expected values
	// Particle #1
	std::vector<std::array<double, 3>> expectedX1s;
	expectedX1s.push_back({ 0.000000000000000, 0.000000000000000, 0.000000000000000 });
	expectedX1s.push_back({ 0.205700000000000, 0.290000000000000, 2.400000000000000 });
	expectedX1s.push_back({ 0.418223755000000, 0.769249500000000, 6.691735000000000 });
	expectedX1s.push_back({ 0.638675572298250, 1.436458638925000, 12.861548555250000 });
	expectedX1s.push_back({ 0.868137164215450, 2.289801870729060, 20.890431697117500 });
	expectedX1s.push_back({ 1.107664565214640, 3.326923167201550, 30.754077108648300 });
	expectedX1s.push_back({ 1.358285122202960, 4.544942732647510, 42.422948400419400 });
	expectedX1s.push_back({ 1.620994566593010, 5.940465211305430, 55.862364289249300 });
	expectedX1s.push_back({ 1.896754176468260, 7.509589364111120, 71.032597439854900 });
	expectedX1s.push_back({ 2.186488036940580, 9.247919187229100, 87.888987687756800 });
	expectedX1s.push_back({ 2.491080406507620, 11.150576440663500, 106.382069320749000 });
	expectedX1s.push_back({ 2.811373196916120, 13.212214551242000, 126.457712056176000 });
	expectedX1s.push_back({ 3.148163573713400, 15.427033850349400, 148.057275312244000 });
	expectedX1s.push_back({ 3.502201684325600, 17.788798102983400, 171.117775333672000 });
	expectedX1s.push_back({ 3.874188520137470, 20.290852281023800, 195.572064695398000 });
	expectedX1s.push_back({ 4.264773918666950, 22.926141530063300, 221.349023672743000 });


	// getting a reference to particles
	auto& engineParticles = engine->getParticlesForUpdate();
	// getting the fixprints
	auto& fix1ref = engine->returnFixById("1");
	// casting the fixPrint
	FixPrint* const fixPrintPtr1 = dynamic_cast<FixPrint*>(fix1ref.get());
	// checking the Ref of the Fix
	REQUIRE(fixPrintPtr1);
	// getting fixprint outputs;
	auto Xs = fixPrintPtr1->getOutputVector();


	std::vector<std::array<double, 3>> expectedV1s;
	expectedV1s.push_back({ 2.000000000000000, 1.000000000000000, 5.000000000000000 });
	expectedV1s.push_back({ 2.057000000000000, 2.900000000000000, 24.000000000000000 });
	expectedV1s.push_back({ 2.125237550000000, 4.792495000000000, 42.917350000000000 });
	expectedV1s.push_back({ 2.204518172982500, 6.672091389250000, 61.698135552500000 });
	expectedV1s.push_back({ 2.294615919172000, 8.533432318040640, 80.288831418675400 });
	expectedV1s.push_back({ 2.395274009991860, 10.371212964724900, 98.636454115307500 });
	expectedV1s.push_back({ 2.506205569883240, 12.180195654459600, 116.688712917711000 });
	expectedV1s.push_back({ 2.627094443900460, 13.955224786579200, 134.394158888299000 });
	expectedV1s.push_back({ 2.757596098752560, 15.691241528057000, 151.702331506055000 });
	expectedV1s.push_back({ 2.897338604723210, 17.383298231179800, 168.563902479020000 });
	expectedV1s.push_back({ 3.045923695670410, 19.026572534343800, 184.930816329919000 });
	expectedV1s.push_back({ 3.202927904084940, 20.616381105784800, 200.756427354277000 });
	expectedV1s.push_back({ 3.367903767972830, 22.148192991074400, 215.995632560676000 });
	expectedV1s.push_back({ 3.540381106122000, 23.617642526339500, 230.605000214277000 });
	expectedV1s.push_back({ 3.719868358118720, 25.020541780404500, 244.542893617268000 });
	expectedV1s.push_back({ 3.905853985294800, 26.352892490395300, 257.769589773449000 });

	// getting the fixprints
	auto& fix2ref = engine->returnFixById("2");
	// casting the fixPrint
	FixPrint* const fixPrintPtr2 = dynamic_cast<FixPrint*>(fix2ref.get());
	// checking the Ref of the Fix
	REQUIRE(fixPrintPtr2);
	// getting fixprint outputs;
	auto Vs = fixPrintPtr2->getOutputVector();

	for (int i = 0; i < nSteps; i++) {
		for (int j = 0; j < 3; j++) {
			//REQUIRE_THAT(Vs[i][j], Catch::Matchers::WithinAbs(expectedV1s[i][j], 1e-4));
			REQUIRE_THAT(Xs[i][j], Catch::Matchers::WithinAbs(expectedX1s[i][j], 1e-2));
		}

	}
}