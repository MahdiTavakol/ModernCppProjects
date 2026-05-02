#include "test_shared.h"
#include "catch_amalgamated.hpp"


TEST_CASE("Testing the default image settings")
{
	int mode;
	std::unique_ptr<image_settings> img_settings;

	int expected_width;
	double expected_height_ratio, expected_width_ratio;


	// default settings
	// each one of the modes
	// overwrites some of the default settings.
	expected_width = 1080;
	expected_height_ratio = 9.0;
	expected_width_ratio = 16.0;




	SECTION("Default settings")
	{
		mode = -1;
	}
	SECTION("Random sphere settings")
	{
		mode = RANDOM_SPHERES;
	}
	SECTION("Quad settings")
	{
		mode = QUADS;
		expected_width = 1080;
	}
	SECTION("Simple light or two lights settings")
	{
		SECTION("Simple light settings")
		{
			mode = SIMPLE_LIGHT;
		}
		SECTION("Two lights settings")
		{
			mode = TWO_LIGHTS;
		}
	}
	SECTION("Cornell_box or two_boxes or two_boxes_rotated")
	{
		SECTION("Cornell_box settings")
		{
			mode = CORNELL_BOX;
		}
		SECTION("Two boxes settings")
		{
			mode = TWO_BOXES;
		}
		SECTION("Two boxes rotated settings")
		{
			mode = TWO_BOXES_ROTATED;
		}
		SECTION("Cornell smoke")
		{
			mode = CORNELL_SMOKE;
		}
		expected_height_ratio = 1.0;
		expected_width_ratio = 1.0;
	}
	SECTION("Random spheres animated")
	{
		mode = RANDOM_SPHERES_ANIMATED;
	}
	SECTION("Simple 2d parallel test")
	{
		mode = SIMPLE_2D_PARALEL_TEST;
		expected_width = 1080;
	}

	img_settings = std::make_unique<image_settings>(mode);

	int image_width = img_settings->get_image_width();
	double width_ratio = img_settings->get_width_ratio();
	double height_ratio = img_settings->get_height_ratio();

	REQUIRE(image_width == expected_width);
	REQUIRE_THAT(width_ratio, Catch::Matchers::WithinAbs(expected_width_ratio, 1e-6));
	REQUIRE_THAT(height_ratio, Catch::Matchers::WithinAbs(expected_height_ratio, 1e-6));
}

TEST_CASE("Testing non-default image_settings")
{
	std::string input;

	// default settings
	int mode = -1;
	// expected defaults
	int expected_width = 1080;
	double expected_height_ratio = 9.0;
	double expected_width_ratio = 16.0;




	SECTION("Test-1")
	{
		input = "image -width 2040 -aspect_ratio 32 12";
		expected_width = 2040;
		expected_width_ratio = 32;
		expected_height_ratio = 12;
	}
	SECTION("Test-2")
	{
		input = "image -width 800 -aspect_ratio 12 14";
		expected_width = 800;
		expected_width_ratio = 12;
		expected_height_ratio = 14;
	}
	SECTION("Test-3")
	{
		input = "image -width 1024";
		expected_width = 1024;
	}
	SECTION("Test-4")
	{
		input = "image -aspect_ratio 4 3";
		expected_width_ratio = 4;
		expected_height_ratio = 3;
	}
	SECTION("Test-5")
	{
		input = "image";
	}
	SECTION("Test-6")
	{
		input = "image   -width   1280   -aspect_ratio   21   9";
		expected_width = 1280;
		expected_width_ratio = 21;
		expected_height_ratio = 9;
	}

	auto img_settings = std::make_unique<image_settings>(mode);
	img_settings->add_cmd(input);
	img_settings->parse_commands();


	int image_width = img_settings->get_image_width();
	double width_ratio = img_settings->get_width_ratio();
	double height_ratio = img_settings->get_height_ratio();

	REQUIRE(image_width == expected_width);
	REQUIRE_THAT(width_ratio, Catch::Matchers::WithinAbs(expected_width_ratio, 1e-6));
	REQUIRE_THAT(height_ratio, Catch::Matchers::WithinAbs(expected_height_ratio, 1e-6));
}
