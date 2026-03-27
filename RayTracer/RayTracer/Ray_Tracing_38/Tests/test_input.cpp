#include "../Input/input.h"
#include "test_shared.h"
#include "catch_amalgamated.hpp"

TEST_CASE("Testing reading the argv with the input class")
{
	std::vector<std::string> argv_vec;
	std::vector<std::string> expectedLog;

	SECTION("Test-1")
	{
		argv_vec.push_back("renderer");
		argv_vec.push_back("-image_width");
		argv_vec.push_back("640");
		argv_vec.push_back("-samples_per_pixel");
		argv_vec.push_back("240");
		argv_vec.push_back("-max_depth");
		argv_vec.push_back("96");
		argv_vec.push_back("-vfov");
		argv_vec.push_back("45");
		argv_vec.push_back("-aspect_ratio");
		argv_vec.push_back("16");
		argv_vec.push_back("9");
		argv_vec.push_back("-fps");
		argv_vec.push_back("120");
		argv_vec.push_back("-num_seconds");
		argv_vec.push_back("85");


		expectedLog.push_back("The input: ");
		expectedLog.push_back("renderer -image_width 640 -samples_per_pixel 240 ");
		expectedLog.push_back("-max_depth 96 -vfov 45 -aspect_ratio ");
		expectedLog.push_back("16 9 -fps 120 -num_seconds ");
		expectedLog.push_back("85 ");
		expectedLog.push_back("");
		expectedLog.push_back("image_width = 640");
		expectedLog.push_back("samples_per_pixel = 240");
		expectedLog.push_back("max_depth = 96");
		expectedLog.push_back("vfov = 45");
		expectedLog.push_back("aspect_ratio = 16/9");
		expectedLog.push_back("fps = 120");
		expectedLog.push_back("num_seconds = 85");
	}

	SECTION("Test-2")
	{
		argv_vec.push_back("renderer");
		argv_vec.push_back("-image_width");
		argv_vec.push_back("1280");
		argv_vec.push_back("-samples_per_pixel");
		argv_vec.push_back("480");
		argv_vec.push_back("-max_depth");
		argv_vec.push_back("35");
		argv_vec.push_back("-vfov");
		argv_vec.push_back("25");
		argv_vec.push_back("-aspect_ratio");
		argv_vec.push_back("4");
		argv_vec.push_back("3");
		argv_vec.push_back("-fps");
		argv_vec.push_back("20");
		argv_vec.push_back("-num_seconds");
		argv_vec.push_back("15");


		expectedLog.push_back("The input: ");
		expectedLog.push_back("renderer -image_width 1280 -samples_per_pixel 480 ");
		expectedLog.push_back("-max_depth 35 -vfov 25 -aspect_ratio ");
		expectedLog.push_back("4 3 -fps 20 -num_seconds ");
		expectedLog.push_back("15 ");
		expectedLog.push_back("");
		expectedLog.push_back("image_width = 1280");
		expectedLog.push_back("samples_per_pixel = 480");
		expectedLog.push_back("max_depth = 35");
		expectedLog.push_back("vfov = 25");
		expectedLog.push_back("aspect_ratio = 4/3");
		expectedLog.push_back("fps = 20");
		expectedLog.push_back("num_seconds = 15");
	}


	std::ostringstream oss;
	std::vector<std::reference_wrapper<std::ostream>> outStream =
	{
		std::ref(oss)
	};

	create_input crt_input(argv_vec, outStream);
	auto in = crt_input.return_input_ptr();


	std::string oss_string = oss.str();
	std::stringstream iss(oss_string);
	for (int i = 0; i < expectedLog.size(); i++)
	{
		std::string output;
		std::getline(iss, output);
		REQUIRE(expectedLog[i] == output);
	}
}