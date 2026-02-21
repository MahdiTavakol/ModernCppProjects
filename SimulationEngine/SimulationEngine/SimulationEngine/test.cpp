#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iostream>


#include "Particles.h"
#include "Engine.h"
#include "EngineFactory.h"
#include "Integrator.h"
#include "EulerIntegrator.h"
#include "Forcefield.h"
#include "Neighbor.h"
#include "SimpleNeighbor.h"
#include "SemiIntegrator.h"
#include "SpringField.h"
#include "FixPrint.h"
#include "Box.h"
#include "Run.h"
#include "Error.h"

using std::vector, std::array;
using std::make_unique, std::unique_ptr;


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
	auto box = make_unique<Box>(mockedEngine, min, max );
	// particles
	auto particles = make_unique<Particles>( mockedEngine,nmax );
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
	auto box = make_unique<Box>(mockedEngine, min, max);
	// particles
	auto particles = make_unique<Particles>(mockedEngine, nmax);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<EulerIntegrator>(mockedEngine);
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
	// creating the run object
	auto run = make_unique<Run>(mockedEngine);
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
	auto box = make_unique<Box>(mockedEngine, min, max);
	// particles
	auto particles = make_unique<Particles>(mockedEngine, nmax);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<EulerIntegrator>(mockedEngine);
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
	finalVs.push_back({10.0,39.9,-210.0 });
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
	// creating the run object
	auto run = make_unique<Run>(mockedEngine);
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
	auto box = make_unique<Box>(mockedEngine, min, max);
	// particles
	auto particles = make_unique<Particles>(mockedEngine, nmax);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<SemiIntegrator>(mockedEngine);
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
	// creating the run object
	auto run = make_unique<Run>(mockedEngine);
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
	auto box = make_unique<Box>(mockedEngine, min, max);
	// particles
	auto particles = make_unique<Particles>(mockedEngine, nmax);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<SemiIntegrator>(mockedEngine);
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
	// creating the run object
	auto run = make_unique<Run>(mockedEngine);
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
	constexpr int nSteps = 5;
	// constant variables
	int nmax = 10;
	array<double, 3> min = { -100000.0,-100000.0,-100000.0 };
	array<double, 3> max = { 100000.0,100000.0,100000.0 };
	// the mocked Engine object
	Engine mockedEngine;
	// box 
	auto box = make_unique<Box>(mockedEngine, min, max);
	// particles
	auto particles = make_unique<Particles>(mockedEngine, nmax);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<SemiIntegrator>(mockedEngine);
	// The fix command
	auto fixPrint1 = make_unique<FixPrint>(mockedEngine, "1", 1, INIT_INTEGRATE,  "x", 0);
	auto fixPrint2 = make_unique<FixPrint>(mockedEngine, "2", 1, INIT_INTEGRATE,  "v", 0);
	auto fixPrint3 = make_unique<FixPrint>(mockedEngine, "3", 1, INIT_INTEGRATE,  "f", 0);
	auto fixPrint4 = make_unique<FixPrint>(mockedEngine, "4", 1, FINAL_INTEGRATE, "x", 0);
	auto fixPrint5 = make_unique<FixPrint>(mockedEngine, "5", 1, FINAL_INTEGRATE, "v", 0);
	auto fixPrint6 = make_unique<FixPrint>(mockedEngine, "6", 1, FINAL_INTEGRATE, "f", 0);
	// adding these to the engine
	mockedEngine.setItem(std::move(box));
	mockedEngine.setItem(std::move(particles));
	mockedEngine.setItem(std::move(integrator));
	mockedEngine.setItem(std::move(fixPrint1));
	mockedEngine.setItem(std::move(fixPrint2));
	mockedEngine.setItem(std::move(fixPrint3));
	mockedEngine.setItem(std::move(fixPrint4));
	mockedEngine.setItem(std::move(fixPrint5));
	mockedEngine.setItem(std::move(fixPrint6));
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

	// adding it 
	for (int i = 0; i < 1; i++) {
		engineParticles->addParticle(newXs[i], newVs[i], newFs[i], newMs[i]);
	}

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


	// creating the run object
	auto run = make_unique<Run>(mockedEngine);
	// setting it up
	run->setup();
	// running it for 100 steps
	run->start(nSteps);
	// getting the fixprints
	auto& fix1ref = mockedEngine.returnFixById("1");
	auto& fix2ref = mockedEngine.returnFixById("2");
	auto& fix3ref = mockedEngine.returnFixById("3");
	auto& fix4ref = mockedEngine.returnFixById("4");
	auto& fix5ref = mockedEngine.returnFixById("5");
	auto& fix6ref = mockedEngine.returnFixById("6");
	// casting the fixPrint
	FixPrint* const fixPrintPtr1 = dynamic_cast<FixPrint*>(fix1ref.get());
	FixPrint* const fixPrintPtr2 = dynamic_cast<FixPrint*>(fix2ref.get());
	FixPrint* const fixPrintPtr3 = dynamic_cast<FixPrint*>(fix3ref.get());
	FixPrint* const fixPrintPtr4 = dynamic_cast<FixPrint*>(fix4ref.get());
	FixPrint* const fixPrintPtr5 = dynamic_cast<FixPrint*>(fix5ref.get());
	FixPrint* const fixPrintPtr6 = dynamic_cast<FixPrint*>(fix6ref.get());
	// checking the Ref of the Fix
	if (fixPrintPtr1 == nullptr ||
		fixPrintPtr2 == nullptr ||
		fixPrintPtr3 == nullptr ||
		fixPrintPtr4 == nullptr ||
		fixPrintPtr5 == nullptr ||
		fixPrintPtr6 == nullptr )
		std::cout << "Error " << std::endl;
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
			REQUIRE_THAT(expectedXs[i][j],   Catch::Matchers::WithinAbs(initXs[i][j],  1e-6));
			REQUIRE_THAT(expectedVs[i][j],   Catch::Matchers::WithinAbs(initVs[i][j],  1e-6));
			REQUIRE_THAT(expectedFs[i][j],   Catch::Matchers::WithinAbs(initFs[i][j],  1e-6));
			REQUIRE_THAT(expectedXs[i+1][j], Catch::Matchers::WithinAbs(finalXs[i][j], 1e-6));
			REQUIRE_THAT(expectedVs[i+1][j], Catch::Matchers::WithinAbs(finalVs[i][j], 1e-6));
			REQUIRE_THAT(expectedFs[i+1][j], Catch::Matchers::WithinAbs(finalFs[i][j], 1e-6));
		}
	}
}


TEST_CASE("Calculating the force from the forcefield equation") {
	std::cout << "Testing the calculation of the force from the forcefield equation" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// number of steps
	constexpr int nSteps = 5;
	// constant variables
	int nmax = 10;
	array<double, 3> min = { -100000.0,-100000.0,-100000.0 };
	array<double, 3> max = { 100000.0,100000.0,100000.0 };
	// the mocked Engine object
	Engine mockedEngine;
	// box 
	auto box = make_unique<Box>(mockedEngine, min, max);
	// particles
	auto particles = make_unique<Particles>(mockedEngine, nmax);
	// forceField
	unique_ptr<ForceField> forcefield = make_unique<SpringField>(mockedEngine,10);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<SemiIntegrator>(mockedEngine);
	// The fix command
	auto fixPrint1 = make_unique<FixPrint>(mockedEngine, "1", 1, POST_FORCE, "f", 0);
	auto fixPrint2 = make_unique<FixPrint>(mockedEngine, "2", 1, POST_FORCE, "f", 1);
	// adding these to the engine
	mockedEngine.setItem(std::move(box));
	mockedEngine.setItem(std::move(particles));
	mockedEngine.setItem(std::move(integrator));
	mockedEngine.setItem(std::move(forcefield));
	mockedEngine.setItem(std::move(fixPrint1));
	mockedEngine.setItem(std::move(fixPrint2));

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

	// adding it 
	for (int i = 0; i < 2; i++) {
		engineParticles->addParticle(newXs[i], newVs[i], newFs[i], newMs[i]);
	}

	// creating the run object
	auto run = make_unique<Run>(mockedEngine);
	// setting it up
	run->setup();
	// running it for 100 steps
	run->start(nSteps);
	// getting the fixprints
	auto& fix1ref = mockedEngine.returnFixById("1");
	auto& fix2ref = mockedEngine.returnFixById("2");

	// casting the fixPrint
	FixPrint* const fixPrintPtr1 = dynamic_cast<FixPrint*>(fix1ref.get());
	FixPrint* const fixPrintPtr2 = dynamic_cast<FixPrint*>(fix2ref.get());
	// checking the Ref of the Fix
	if (fixPrintPtr1 == nullptr ||
		fixPrintPtr2 == nullptr)
		std::cout << "Error " << std::endl;

	// getting fixprint outputs;
	auto F1s = fixPrintPtr1->getOutputVector();
	auto F2s = fixPrintPtr2->getOutputVector();


	// expected values
	vector<array<double, 3>> expectedF1s, expectedF2s;

	// Particle #1
	expectedF1s.push_back({  30.0,1000.0,10000.0 });
	expectedF1s.push_back({ -30.0,-3011.0,-30150.0 });
	expectedF1s.push_back({  30.0, 5022.0, 50300.0 });
	expectedF1s.push_back({ -30.0,-7033.0,-70450.0 });
	expectedF1s.push_back({  30.0, 9044.0, 90600.0 });
	expectedF1s.push_back({ -30.0,-11055.0,-110750.0 });
	// Particle #2
	for (auto& f : expectedF1s)
		expectedF2s.push_back({ -f[0],-f[1],-f[2] });

	// comparing the results
	for (int i = 0; i < nSteps; i++) {
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
		Engine::Run_Status status;
	} run_array[2] = {
		{"Running the silent version of the engine",
		 Engine::Run_Status::SILENT},
		{"Running the verbose version of the engine",
		 Engine::Run_Status::VERBOSE},
	} ;


	for (auto& run : run_array) {
		std::cout << run.messages << std::endl;
		constexpr int nSteps = 5;
		// constant variables
		int nmax = 10;
		array<double, 3> min = { -100000.0,-100000.0,-100000.0 };
		array<double, 3> max = { 100000.0,100000.0,100000.0 };
		// the mocked Engine object
		Engine mockedEngine(run.status);
		// box 
		auto box = make_unique<Box>(mockedEngine, min, max);
		// particles
		auto particles = make_unique<Particles>(mockedEngine, nmax);
		// Integrator
		unique_ptr<Integrator> integrator = make_unique<SemiIntegrator>(mockedEngine);
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

		// adding it 
		for (int i = 0; i < 2; i++) {
			engineParticles->addParticle(newXs[i], newVs[i], newFs[i], newMs[i]);
		}

		// creating the run object
		auto run = make_unique<Run>(mockedEngine);
		// setting it up
		run->setup();
		// running it for 100 steps
		run->start(nSteps);
	}
}

TEST_CASE("Testing the neighbor list construction and updating") {
	std::cout << "Testing the neighbor list construction and updating" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	constexpr int nSteps = 5;
	// constant variables
	int nmax = 10;
	// max and min
	array<double, 3> min = { -100000.0,-100000.0,-100000.0 };
	array<double, 3> max = { 100000.0,100000.0,100000.0 };
	// neighbor cutoff
	double neighbor_cutoff = 90.0;
	//
	Engine::Run_Status run_status = Engine::Run_Status::SILENT;
	// the mocked Engine object
	Engine mockedEngine(run_status);
	// box 
	auto box = make_unique<Box>(mockedEngine, min, max);
	// particles
	auto particles = make_unique<Particles>(mockedEngine, nmax);
	// Integrator
	unique_ptr<Integrator> integrator = make_unique<SemiIntegrator>(mockedEngine);
	// neighbor list
	unique_ptr<Neighbor> neighbor = make_unique<SimpleNeighbor>(mockedEngine, neighbor_cutoff);
	// adding these to the engine
	mockedEngine.setItem(std::move(box));
	mockedEngine.setItem(std::move(particles));
	mockedEngine.setItem(std::move(integrator));
	mockedEngine.setItem(std::move(neighbor));

	// getting a reference to particles
	auto& engineParticles = mockedEngine.getParticlesForUpdate();
	// new particles
	std::vector<std::array<double, 3>> newXs = {
	{ -24.0, -94.0, -37.0 },
	{ -27.0, -20.0, -25.0 },
	{ -49.0,  98.0,  96.0 },
	{  13.0,  -3.0,  23.0 },
	{  22.0, -27.0,  89.0 },
	{  97.0,  19.0,  80.0 },
	{ -10.0,  14.0, -23.0 },
	{ -79.0, -89.0, -71.0 },
	{ 100.0, -58.0, -90.0 },
	{ -86.0,  32.0,  85.0 }
	};
	vector<array<double, 3>> newVs, newFs;
	vector<double> newMs;
	for (int i = 0; i < 10; i++) {
		newVs.push_back({ 0.0,0.0,0.0 });
		newFs.push_back({ 0.0,0.0,0.0 });
		newMs.push_back(5.0);
	}
	std::map<int, std::vector<int>> expectedNeighborList = {
	{1, {2, 8}},
	{2, {1, 4, 7}},
	{3, {10}},
	{4, {2, 5, 7}},
	{5, {4, 6}},
	{6, {5}},
	{7, {2, 4}},
	{8, {1}},
	{10,{3}}
	};
	// adding it 
	for (int i = 0; i < 10; i++) {
		engineParticles->addParticle(newXs[i], newVs[i], newFs[i], newMs[i]);
	}

	// creating the run object
	auto run = make_unique<Run>(mockedEngine);
	// setting it up
	run->setup();
	// running it for 100 steps
	run->start(nSteps);
	// getting the neighbor list
	auto& neighborPtr = mockedEngine.getNeighbor();
	int nNeigh = 0;
	int* neighList = nullptr, * firstNeigh = nullptr, * numNeigh = nullptr;
	neighborPtr->getNeighborList(nNeigh, neighList,  firstNeigh, numNeigh);
	// comparing the results
	for (int i = 0;i < nNeigh; i++) {
		 std::cout << std::endl;
		int particleId = i + 1;
		std::vector<int> expectedNeighbors = expectedNeighborList[particleId];
		std::vector<int> actualNeighbors(neighList + firstNeigh[i], neighList + firstNeigh[i] + numNeigh[i]);
		for (int& neighborId : actualNeighbors) {
			neighborId += 1; // converting from 0-based to 1-based indexing
		}
		REQUIRE_THAT(actualNeighbors, Catch::Matchers::UnorderedEquals(expectedNeighbors));
	}
}

TEST_CASE("Testing various error and warning messages")
{
	std::cout << "Testing various error and warning messages" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// constant variables
	int nmax = 10;
	array<double, 3> min = { 0.0,0.0,0.0 };
	array<double, 3> max = { 10.0,10.0,10.0 };
	// the mocked Engine object
	Engine mockedEngine;
	// box 
	auto box = make_unique<Box>(mockedEngine, min, max);
	// particles
	// Error streams
	std::ostringstream errorStream1, errorStream2, errorStream3;
	std::vector<std::reference_wrapper<std::ostream>> errorStreams = {
		std::ref(errorStream1),
		std::ref(errorStream2),
		std::ref(errorStream3)
	};
	auto error = make_unique<Error>(mockedEngine, errorStreams);
	// setting items into the engine
	mockedEngine.setItem(std::move(box));
	mockedEngine.setItem(std::move(error));
	// get error reference from the engine
	auto& errorRef = mockedEngine.getError();
	// invoking the error function with different messages
	std::cout << "Using single error and warning messages:" << std::endl;
	errorRef->one("Error message 1");
	errorRef->warning("Warning message 1");
	errorRef->one("Error message 2");
	errorRef->warning("Warning message 2");
	errorRef->one("Error message 3");
	errorRef->warning("Warning message 3");
	// checking the outputs
	std::cout << "The ostringstreams contain:" << std::endl;
	auto& errorStreamsRef = errorRef->getStreams();
	for (auto& stream : errorStreamsRef) {
		if (auto ostringstreamPtr = dynamic_cast<std::ostringstream*>(&(stream.get()))) {
			std::cout << ostringstreamPtr->str() << std::endl;
		}
		else
			std::cout << "Error: Stream is not an ostringstream" << std::endl;
	}
	
	std::vector<std::reference_wrapper<std::ostream>> newErrorStreams = {
		std::ref(std::cout)
	};
	errorRef->setStreams(newErrorStreams);
	std::cout << "Chaining the warning messages:" << std::endl;
	(*errorRef) << "Chained warning message 1" << std::endl
				<< "Chained warning message 2" << std::endl
		        << "Chained warning message 3" << std::endl;
	SUCCEED("Error and warning messages were successfully written to the provided ostringstreams");
}

TEST_CASE("Testing the engine factory class to build the engine from commands") {
	std::cout << "Testing the engine factory class to build the engine from commands" << std::endl;
	std::cout << std::string(80, '=') << std::endl;
	// the commands vector to build the engine
	std::vector<std::string> commands = {
		"box 0.0 0.0 0.0 100000.0 100000.0 100000.0",
		"particles 10",
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
	EngineFactory factory(commands);
	// building the engine using the factory
	auto engine = factory.returnEngine();
	// checking if the engine was built successfully
	
	// 1 - checking the box dimensions
	auto& boxRef = engine.getBox();
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
	auto& particlesRef = engine.getParticles();
	REQUIRE(particlesRef);
	int nmax, nlocal;
	particlesRef->getNmaxNlocal(nmax, nlocal);
	REQUIRE(nmax == 10);

	// 3 - checking the integrator type
	Integrator* integratorRaw = engine.getIntegrator().get();
	REQUIRE(integratorRaw);
	if (auto semiIntegratorPtr = dynamic_cast<SemiIntegrator*>(integratorRaw)) {
		SUCCEED("The integrator is of type SemiIntegrator");
	}
	else {
		FAIL("The integrator is not of type SemiIntegrator");
	}

	// 4 - checking the error streams
	auto& errorRef = engine.getError();
	REQUIRE(errorRef);
	auto& errorStreams = errorRef->getStreams();
	REQUIRE(errorStreams.size() == 1);
	if (&errorStreams[0].get() == & std::cout) {
		SUCCEED("The error stream is set to std::cout");
	}
	else {
		FAIL("The error stream is not set to std::cout");
	}

	// 5 - checking the fix prints
	Fix* fix1Ref = engine.returnFixById("1").get();
	Fix* fix2Ref = engine.returnFixById("2").get();
	REQUIRE(fix1Ref);
	REQUIRE(fix2Ref);
	if (auto fixPtr = dynamic_cast<FixPrint*>(fix1Ref)
		&& dynamic_cast<FixPrint*>(fix2Ref)) {
		SUCCEED("The fixs with ids print1 and print2 are of type print");
	}
	else {
		FAIL("The fixs with ids print1 and print2 are not of type print");
	}

	// 6 - checking the force field
	//auto& forcefield = engine.getForceField();
	//REQUIRE(forcefield);
	//if (auto springFieldPtr = dynamic_cast<SpringField*>(forcefield.get())) {
	//	SUCCEED("The force field is of type SpringField");
	//}
	//else {
	//	FAIL("The force field is not of type SpringField");
	//}

	// 7 - checking the neighbor
	// auto& neighbor = engine.getNeighbor();
	//REQUIRE(neighbor);
	//double expectedCutoff = 90.0;
	//double actualCutoff = neighbor->getCutoff();
	//REQUIRE(expectedCutoff == actualCutoff);
	//if (auto simpleNeighborPtr = dynamic_cast<SimpleNeighbor*>(neighbor.get())) {
	//	SUCCEED("The neighbor is of type SimpleNeighbor");
	//}
	//else {
	//	FAIL("The neighbor is not of type SimpleNeighbor");
	//}

	// 8 - checking the run status
	auto& runStatus = engine.getStatus();
	REQUIRE(runStatus == Engine::Run_Status::VERBOSE);

	// 9 - checking the run command
	auto& runCommand = engine.getRunCommand();
	REQUIRE(runCommand);
	int expectedSteps = 100;
	int actualSteps = runCommand->getSteps();
	REQUIRE(expectedSteps == actualSteps);
}