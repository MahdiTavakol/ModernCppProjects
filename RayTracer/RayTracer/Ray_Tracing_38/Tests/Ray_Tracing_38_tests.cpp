#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include "../Input/input.h"
#include <sstream>

TEST_CASE("Testing reading the argv with the input class")
{
	std::vector<std::string> argv_vec;
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

	int argc = static_cast<int>(argv_vec.size());
	char** argv;
	argv = new char* [argc];
	for (int iarg = 0; iarg < argc; iarg++)
	{
		int size = static_cast<int>(argv_vec[iarg].size());
		argv[iarg] = new char[size+1];
		std::memcpy(argv[iarg], argv_vec[iarg].c_str(), size+1);
	}

	std::ostringstream oss;
	std::vector<std::reference_wrapper<std::ostream>> outStream =
	{
		std::ref(oss)
	};
	std::unique_ptr<input> in = std::make_unique<input>(argc, argv, 0,outStream);

	std::vector<std::string> expectedLog;
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


	std::string oss_string = oss.str();
	std::stringstream iss(oss_string);
	for (int i = 0; i < expectedLog.size(); i++)
	{
		std::string output;
		std::getline(iss, output);
 		REQUIRE(expectedLog[i] == output);
	}


	for (int iarg = 0; iarg < argc; iarg++)
	{
		delete[] argv[iarg];
	}
	delete[] argv;
}

