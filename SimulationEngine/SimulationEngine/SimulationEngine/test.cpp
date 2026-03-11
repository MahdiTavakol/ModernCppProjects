#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iostream>

#include "test.hpp"




TEST_CASE("Testing the simple neighbor class")
{
	std::cout << "Testing the simple neighbor class" << std::endl;
	std::cout << std::string(80, '=') << std::endl;


	std::vector<double> x =
	{
		-280.0, 0.0, 0.0,
		-260.0,5.0,0.0,
		-240.0,-5.0,10.0,
		-50.0,0.0,0.0,
		-30.0,10.0,0.0,
		-10.0,-5.0,0.0,
		15.0,0.0,5.0,
		40.0,-5.0,0.0,
		200.0,0.0,0.0,
		235.0,0.0,0.0
	};


}

