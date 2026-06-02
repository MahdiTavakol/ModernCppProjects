#include "test_shared.h"
#include "catch_amalgamated.hpp"


TEST_CASE("Testing the default camera settings")
{
	int mode;

	std::unique_ptr<camera_settings> cam_settings;


	int expected_samples_per_pixel;
	int expected_max_depth, expected_vfov;
	double expected_defocus_angle, expected_focus_dist;
	point3 expected_lookat;
	point3 expected_vup;
	color expected_background;
	int expected_fps;
	int expected_num_seconds;


	// default settings
	// each one of the modes
	// overwrites some of the default settings.
	expected_samples_per_pixel = 150;
	expected_max_depth = 50;
	expected_vfov = 45;
	expected_defocus_angle = 0.6;
	expected_focus_dist = 10.0;
	expected_lookat = point3(0, 0, 0);
	expected_vup = point3(0, 1, 0);
	expected_background = color(0.7, 0.8, 1.00);
	expected_fps = 1;
	expected_num_seconds = 1;




	SECTION("Default settings")
	{
		mode = -1;
	}
	SECTION("Random sphere settings")
	{
		mode = RANDOM_SPHERES;
		expected_vfov = 20;
		expected_lookat = point3{ 0,0,0 };
		expected_samples_per_pixel = 100;
		expected_defocus_angle = 0;
	}
	SECTION("Quad settings")
	{
		mode = QUADS;
		expected_samples_per_pixel = 100;
		expected_max_depth = 50;
		expected_vfov = 80;
		expected_lookat = point3(0, 0, 0);
		expected_vup = vec3(0, 1, 0);
		expected_defocus_angle = 0;
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
		expected_lookat = point3(0, 1, 0);
		expected_defocus_angle = 0;
		expected_vfov = 20;
		expected_background = color(0, 0, 0);
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
		expected_lookat = point3(278, 278, 0);
		expected_vfov = 40;
		expected_vup = vec3(0, 1, 0);
		expected_defocus_angle = 0;
		expected_background = color(0, 0, 0);
	}
	SECTION("Cornell smoke settings")
	{
		mode = CORNELL_SMOKE;
		expected_background = color(0, 0, 0);
		expected_vfov = 40;
		expected_lookat = point3(278, 278, 0);
		expected_vup = vec3(0, 1, 0);
		expected_defocus_angle = 0;
	}
	SECTION("Random spheres animated settings")
	{
		mode = RANDOM_SPHERES_ANIMATED;
		expected_fps = 60;
		expected_num_seconds = 10;
		expected_background = color(0.7, 0.8, 1.00);
	}
	SECTION("Simple 2D parallel test settings")
	{
		mode = SIMPLE_2D_PARALEL_TEST;
		expected_background = color(0.7, 0.8, 1.00);
		expected_lookat = point3(0, 0, 0);
		expected_vup = vec3(0, 1, 0);
		expected_vfov = 20;
		expected_defocus_angle = 0;
	}

	cam_settings = std::make_unique<camera_settings>(mode);

	int samples_per_pixel = cam_settings->get_samples_per_pixel();
	int max_depth = cam_settings->get_max_depth();
	int vfov = cam_settings->get_vfov();
	double defocus_angle = cam_settings->get_defocus_angle();
	double focus_dist = cam_settings->get_focus_dist();
	point3 lookat = cam_settings->get_lookat();
	point3 vup = cam_settings->get_vup();
	color background = cam_settings->get_background();


	REQUIRE(samples_per_pixel == expected_samples_per_pixel);
	REQUIRE(max_depth == expected_max_depth);
	REQUIRE(vfov == expected_vfov);
	REQUIRE_THAT(defocus_angle, Catch::Matchers::WithinAbs(expected_defocus_angle, 1e-6));
	REQUIRE_THAT(focus_dist, Catch::Matchers::WithinAbs(expected_focus_dist, 1e-6));
	REQUIRE_THAT(lookat, point3Matcher(expected_lookat, 1e-6));
	REQUIRE_THAT(vup, point3Matcher(expected_vup, 1e-6));
	REQUIRE_THAT(background, point3Matcher(expected_background, 1e-6));
}

TEST_CASE("Testing non-default camera_settings")
{
	std::string input;

	// default settings
	int mode = -1;
	auto cam_settings = std::make_unique<camera_settings>(mode);
	// expected defaults
	int exp_samples_per_pixel = 150;
	int exp_max_depth = 50;
	int exp_vfov = 45;
	double exp_defocus_angle = 0.6;
	double exp_focus_dist = 10.0;
	point3 exp_lookat = point3(0, 0, 0);
	point3 exp_vup = point3(0, 1, 0);
	color exp_background = color(0.7, 0.8, 1.00);
	


	SECTION("Test-1")
	{
		input = "camera -samples_per_pixel 72 -max_depth 10 -vfov 25";
		exp_samples_per_pixel = 72;
		exp_max_depth = 10;
		exp_vfov = 25;
	}
	SECTION("Test-2")
	{
		input = "camera -samples_per_pixel 120 -max_depth 50 -vfov 120";
		input += " -defocus_angle 12 -focus_dist 521 -lookat 12 18.5 19.8";
		input += " -vup 12 16.8 65.4 -background 14.0 1.2 6.47";
		exp_samples_per_pixel = 120;
		exp_max_depth = 50;
		exp_vfov = 120;
		exp_defocus_angle = 12.0;
		exp_focus_dist = 521.0;
		exp_lookat = point3{ 12.0,18.5,19.8 };
		exp_vup = point3{ 12,16.8,65.4 };
		exp_background = point3{ 14.0,1.2,6.47 };
	}
	SECTION("Test-3")
	{
		input = "camera -samples_per_pixel 200 -max_depth 25 -vfov 90";
		input += " -lookat 1 2 3";
		input += " -vup 0 1 0 -background 0.1 0.2 0.3";

		exp_samples_per_pixel = 200;
		exp_max_depth = 25;
		exp_vfov = 90;

		exp_lookat = point3{ 1, 2, 3 };
		exp_vup = point3{ 0, 1, 0 };
		exp_background = point3{ 0.1, 0.2, 0.3 };
	}
	SECTION("Test-4")
	{
		input = "camera -samples_per_pixel 80 -max_depth 10 -vfov 60";
		input += " -defocus_angle 5 -focus_dist 100";
		input += " -background 1.0 1.0 1.0";

		exp_samples_per_pixel = 80;
		exp_max_depth = 10;
		exp_vfov = 60;
		exp_defocus_angle = 5.0;
		exp_focus_dist = 100.0;

		exp_background = point3{ 1.0, 1.0, 1.0 };
	}
	SECTION("Test-5")
	{
		input = "camera -samples_per_pixel 10 -max_depth 5 -vfov 45";

		exp_samples_per_pixel = 10;
		exp_max_depth = 5;
		exp_vfov = 45;
	}
	SECTION("Test-6")
	{
		input = "camera -samples_per_pixel 150 -max_depth 40 -vfov 75";
		input += " -defocus_angle 3 -focus_dist 250";
		input += " -lookat 5 6 7 -vup 0 1 0";

		exp_samples_per_pixel = 150;
		exp_max_depth = 40;
		exp_vfov = 75;
		exp_defocus_angle = 3.0;
		exp_focus_dist = 250.0;
		exp_lookat = point3{ 5, 6, 7 };
		exp_vup = point3{ 0, 1, 0 };
	}


	cam_settings->add_cmd(input);
	cam_settings->parse_commands();


	int samples_per_pixel = cam_settings->get_samples_per_pixel();
	int max_depth = cam_settings->get_max_depth();
	int vfov = cam_settings->get_vfov();
	double defocus_angle = cam_settings->get_defocus_angle();
	double focus_dist = cam_settings->get_focus_dist();
	point3 lookat = cam_settings->get_lookat();
	point3 vup = cam_settings->get_vup();
	color background = cam_settings->get_background();


	REQUIRE(samples_per_pixel == exp_samples_per_pixel);
	REQUIRE(max_depth == exp_max_depth);
	REQUIRE(vfov == exp_vfov);
	REQUIRE_THAT(defocus_angle, Catch::Matchers::WithinAbs(exp_defocus_angle, 1e-6));
	REQUIRE_THAT(focus_dist, Catch::Matchers::WithinAbs(exp_focus_dist, 1e-6));
	REQUIRE_THAT(lookat, point3Matcher(exp_lookat, 1e-6));
	REQUIRE_THAT(vup, point3Matcher(exp_vup, 1e-6));
	REQUIRE_THAT(background, point3Matcher(exp_background, 1e-6));
}