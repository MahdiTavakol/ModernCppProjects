#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iostream>


#include "Particles.h"
#include "Engine.h"
#include "Integrator.h"
#include "EulerIntegrator.h"
#include "Forcefield.h"
#include "SemiIntegrator.h"
#include "SpringField.h"
#include "FixPrint.h"
#include "Box.h"
#include "Run.h"

using std::vector, std::array;
using std::make_unique, std::unique_ptr;


TEST_CASE("Starting and registering each class of the Engine class with minimal input args")
{
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
	mockedEngine.registerBox(box);
	mockedEngine.registerParticles(particles);
	// parameters afer registering into the engine
	const unique_ptr<Box>& boxE = mockedEngine.getBox();
	const unique_ptr<Particles>& particlesE = mockedEngine.getParticles();
	boxE->getDimensions(rMin2, rMax2);
	particlesE->getNmaxNlocal(rNmax2, rNlocal2);
	// checking if they are equal
	REQUIRE(rMin1 == rMin2);
	REQUIRE(rMax1 == rMax2);
	REQUIRE(rNmax1 == rNmax2);
	REQUIRE(rNlocal1 == rNlocal2);

}

TEST_CASE("Testing the Euler integration in 100 steps for 3 particles under constant velocity condition")
{
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
	mockedEngine.registerBox(box);
	mockedEngine.registerParticles(particles);
	mockedEngine.registerIntegrator(integrator);
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
	mockedEngine.registerBox(box);
	mockedEngine.registerParticles(particles);
	mockedEngine.registerIntegrator(integrator);
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
	mockedEngine.registerBox(box);
	mockedEngine.registerParticles(particles);
	mockedEngine.registerIntegrator(integrator);
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
	mockedEngine.registerBox(box);
	mockedEngine.registerParticles(particles);
	mockedEngine.registerIntegrator(integrator);
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
	mockedEngine.registerBox(box);
	mockedEngine.registerParticles(particles);
	mockedEngine.registerIntegrator(integrator);
	mockedEngine.registerFix(std::move(fixPrint1));
	mockedEngine.registerFix(std::move(fixPrint2));
	mockedEngine.registerFix(std::move(fixPrint3));
	mockedEngine.registerFix(std::move(fixPrint4));
	mockedEngine.registerFix(std::move(fixPrint5));
	mockedEngine.registerFix(std::move(fixPrint6));
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
	// checking the type of the Fix
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
	mockedEngine.registerBox(box);
	mockedEngine.registerParticles(particles);
	mockedEngine.registerForceField(forcefield);
	mockedEngine.registerIntegrator(integrator);
	mockedEngine.registerFix(std::move(fixPrint1));
	mockedEngine.registerFix(std::move(fixPrint2));

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

	// adding it 
	for (int i = 0; i < 1; i++) {
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
	// checking the type of the Fix
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
	expectedF1s.push_back({  90.0, 989.0,9850.0 });
	expectedF1s.push_back({ -90.0,-3000.0,-30000.0 });
	expectedF1s.push_back({-270.0,-6869.0,-69850.0 });
	expectedF1s.push_back({-270.0,-4978.0,-49700.0 });
	expectedF1s.push_back({ 270.0,11011.0,110150.0 });
	// Particle #2
	expectedF2s.push_back({ -30.0,-1000.0,-10000.0 });
	expectedF2s.push_back({  -90.0, -989.0,-9850.0 });
	expectedF2s.push_back({   90.0, 3000.0, 30000.0 });
	expectedF2s.push_back({  270.0, 6869.0, 69850.0 });
	expectedF2s.push_back({  270.0, 4978.0, 49700.0 });
	expectedF2s.push_back({ -270.0,-11011.0,-110150.0 });


	// comparing the results
	for (int i = 0; i < nSteps; i++) {
		for (int j = 0; j < 3; j++) {
			REQUIRE_THAT(expectedF1s[i][j], Catch::Matchers::WithinAbs(F1s[i][j], 1e-6));
			//REQUIRE_THAT(expectedF2s[i][j], Catch::Matchers::WithinAbs(F2s[i][j], 1e-6));
		}
	}

}
