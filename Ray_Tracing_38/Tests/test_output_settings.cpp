#include "test_shared.h"
#include "catch_amalgamated.hpp"
#include "../Input/output_settings.h"


TEST_CASE("Testing the default output settings")
{
	int mode;
	std::unique_ptr<output_settings> out_settings;

	outputType expected_type = outputType::SERIAL;
	outputMode expected_outMode = outputMode::P3;


	SECTION("Default settings")
	{
		mode = -1;
	}
	SECTION("Simple 2d parallel test")
	{
		mode = SIMPLE_2D_PARALEL_TEST;
		expected_type = outputType::SERIAL;
		expected_outMode = outputMode::P3;
	}

	out_settings = std::make_unique<output_settings>(mode);
	out_settings->extra_parse();

	outputType type = out_settings->return_type();
	outputMode outMode = out_settings->return_outputMode();

	REQUIRE(type == expected_type);
	REQUIRE(outMode == expected_outMode);
}

TEST_CASE("Testing non-default output_settings")
{
	std::string input;

	// default settings
	int mode = -1;
	// expected defaults
	outputType expected_type = outputType::SERIAL;
	outputMode expected_outMode = outputMode::P3;
	std::string expected_file = "";




	SECTION("Test-1")
	{
		input = "output -output_type PARALLEL -output_mode P6 -image_file test1.ppm";
		expected_type = outputType::PARALLEL;
		expected_outMode = outputMode::P6;
		expected_file = "test1.ppm";
	}
	SECTION("Test-2")
	{
		input = "output -output_type SERIAL -output_mode P3 -image_file test2.ppm";
		expected_type = outputType::SERIAL;
		expected_outMode = outputMode::P3;
		expected_file = "test2.ppm";
	}
	SECTION("Test-3")
	{
		input = "output -output_type PARALLEL -output_mode P3 -image_file test3.ppm";
		expected_type = outputType::PARALLEL;
		expected_outMode = outputMode::P3;
		expected_file = "test3.ppm";
	}
	SECTION("Test-4")
	{
		input = "output -output_type SERIAL -image_file test4.ppm";
		expected_type = outputType::SERIAL;
		expected_file = "test4.ppm";
	}
	SECTION("Test-5")
	{
		input = "output -output_mode P6 -image_file test5.ppm";
		expected_outMode = outputMode::P6;
		expected_file = "test5.ppm";
	}

	auto out_settings = std::make_unique<output_settings>(mode);

	out_settings->add_cmd(input);
	out_settings->parse_commands();
	out_settings->extra_parse();


	outputType type = out_settings->return_type();
	outputMode outMode = out_settings->return_outputMode();
	std::string file = out_settings->get_file_name();

	REQUIRE(type == expected_type);
	REQUIRE(outMode == expected_outMode);
	REQUIRE(file == expected_file);
}
