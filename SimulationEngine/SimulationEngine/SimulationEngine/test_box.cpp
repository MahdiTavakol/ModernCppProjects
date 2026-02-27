#include "catch_amalgamated.hpp"
#include "test.hpp"
#include <iostream>

// box unittests
TEST_CASE("Testing a box object") {
	std::cout << "Testing the box object" << std::endl;
	std::cout << std::string(80, '=') << std::endl;

	// Engine
	Engine engine;
	// box dimensions
	array<double, 3> min = { 0.0,0.0,0.0 };
	array<double, 3> max = { 10.0,10.0,10.0 };
	// box 
	auto box = make_unique<Box>(min, max);
	// injecting the dependencies
	box->injectDependencies(engine);
	// registering the box
	engine.setItem(std::move(box));
	// getting the box
	auto& boxPtr = engine.getBox();
	// getting rmin and rmax
	array<double, 3> bMin;
	array<double, 3> bMax;
	boxPtr->getDimensions(bMin, bMax);
	// checking if they are equal
	REQUIRE(min == bMin);
	REQUIRE(max == bMax);
}