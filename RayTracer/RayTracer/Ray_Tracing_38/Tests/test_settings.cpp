#include "../Input/settings.h"
#include "test_shared.h"
#include "catch_amalgamated.hpp"

// here we just test the parse_inputs functions
// the parse_commands functions are tested in the 
// test for each class derive from the settings class

constexpr double tol = 1e-6;

TEST_CASE("Testing the settings class parse_inputs functions")
{
	fake_settings sett{ 0 };
	std::istringstream ss;

	SECTION("one int")
	{
		std::string input_string;
		ss = std::istringstream("12");
		int param1;
		int expected1 = 12;
		sett.parse_input<int>(ss, param1);
		REQUIRE(param1 == expected1);
	}

	SECTION("two ints")
	{
		std::string input_string;
		ss = std::istringstream("57 89");
		int param1, param2;
		int expected1 = 57, expected2 = 89;
		sett.parse_input<int>(ss, param1, param2);
		REQUIRE(param1 == expected1);
		REQUIRE(param2 == expected2);
	}

	SECTION("three ints")
	{
		std::string input_string;
		ss = std::istringstream("10 20 30");
		int param1, param2, param3;
		int expected1{ 10 }, expected2{ 20 }, expected3{ 30 };
		sett.parse_input<int>(ss, param1, param2, param3);
		REQUIRE(param1 == expected1);
		REQUIRE(param2 == expected2);
		REQUIRE(param3 == expected3);
	}

	SECTION("one float")
	{
		std::string input_string;
		ss = std::istringstream("12.8");
		float param1;
		float expected1 = 12.8;
		sett.parse_input<float>(ss, param1);
		REQUIRE_THAT(param1, Catch::Matchers::WithinAbs(expected1,tol));
	}

	SECTION("two float")
	{
		std::string input_string;
		ss = std::istringstream("57.89 89.25");
		float param1, param2;
		float expected1 = 57.89, expected2 = 89.25;
		sett.parse_input<float>(ss, param1, param2);
		REQUIRE_THAT(param1, Catch::Matchers::WithinAbs(expected1, tol));
		REQUIRE_THAT(param2, Catch::Matchers::WithinAbs(expected2, tol));
	}

	SECTION("three floats")
	{
		std::string input_string;
		ss = std::istringstream("10.76 20.36 30.97");
		float param1, param2, param3;
		float expected1{ 10.76 }, expected2{ 20.36 }, expected3{ 30.97 };
		sett.parse_input<float>(ss, param1, param2, param3);
		REQUIRE_THAT(param1, Catch::Matchers::WithinAbs(expected1, tol));
		REQUIRE_THAT(param2, Catch::Matchers::WithinAbs(expected2, tol));
		REQUIRE_THAT(param3, Catch::Matchers::WithinAbs(expected3, tol));
	}

	SECTION("one double")
	{
		std::string input_string;
		ss = std::istringstream("12.8");
		double param1;
		double expected1 = 12.8;
		sett.parse_input<double>(ss, param1);
		REQUIRE_THAT(param1, Catch::Matchers::WithinAbs(expected1, tol));
	}

	SECTION("two doubles")
	{
		std::string input_string;
		ss = std::istringstream("57.89 89.25");
		double param1, param2;
		double expected1 = 57.89, expected2 = 89.25;
		sett.parse_input<double>(ss, param1, param2);
		REQUIRE_THAT(param1, Catch::Matchers::WithinAbs(expected1, tol));
		REQUIRE_THAT(param2, Catch::Matchers::WithinAbs(expected2, tol));
	}

	SECTION("three doubles")
	{
		std::string input_string;
		ss = std::istringstream("10.76 20.36 30.97");
		double param1, param2, param3;
		double expected1{ 10.76 }, expected2{ 20.36 }, expected3{ 30.97 };
		sett.parse_input<double>(ss, param1, param2, param3);
		REQUIRE_THAT(param1, Catch::Matchers::WithinAbs(expected1, tol));
		REQUIRE_THAT(param2, Catch::Matchers::WithinAbs(expected2, tol));
		REQUIRE_THAT(param3, Catch::Matchers::WithinAbs(expected3, tol));
	}


}