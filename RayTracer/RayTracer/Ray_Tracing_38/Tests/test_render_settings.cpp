#include "test_shared.h"
#include "catch_amalgamated.hpp"
#include "../Input/renderer_settings.h"


TEST_CASE("Testing the default renderer settings")
{
	int mode;
	std::unique_ptr<renderer_settings> render_settings;


	renderMode expected_render_mode = renderMode::STATIC;
	Path_type expected_path_type = Path_type::NONE;
	point3 expected_location = point3(13.0, 2.0, 3.0);
	point3 expected_center{0.0,0.0,0.0};
	double expected_radius{ 0.0};
	double expected_theta{ 0.0 };
	std::string file_name = "";
	int expected_fps = 0;
	int expected_num_seconds = 0;


	SECTION("Default settings")
	{
		mode = -1;
	}
	SECTION("Quads")
	{
		mode = QUADS;
		expected_location = point3(0, 0, 9);
	}
	SECTION("Two lights or simple light")
	{
		SECTION("Two lights")
		{
			mode = TWO_LIGHTS;
		}
		SECTION("Simple light")
		{
			mode = SIMPLE_LIGHT;
		}
		expected_location = point3(26,3,6);
	}
	SECTION("Cornell_box or two_boxes or two_boxes_rotated")
	{
		SECTION("Cornell_box")
		{
			mode = CORNELL_BOX;
		}
		SECTION("Two_box")
		{
			mode = TWO_BOXES;
		}
		SECTION("Two_boxes rotated")
		{
			mode = TWO_BOXES_ROTATED;
		}
		expected_location = point3(278, 278, -800);
	}
	SECTION("Cornell smoke")
	{
		mode = CORNELL_SMOKE;
		expected_location = point3(278, 278, -800);
	}
	SECTION("Simple 2D parallel test")
	{
		mode = SIMPLE_2D_PARALEL_TEST;
		expected_location = point3(100, 100, 500);
	}
	SECTION("Random_spheres animated")
	{
		mode = RANDOM_SPHERES_ANIMATED;
		expected_center = point3{ 13, 2, 3 };
		expected_radius = 13.49;
		expected_theta = 45.0;
		expected_fps = 60;
		expected_num_seconds = 10;
	}
	SECTION("Random spheres")
	{
		mode = RANDOM_SPHERES;
		expected_location = point3{ 0,3,12 };
	}
	SECTION("The rest")
	{
		SECTION("Checker boards")
		{
			mode = CHECKER_BOARDS;
		}
		SECTION("Earth sphere")
		{
			mode = EARTH_SPHERE;
		}
		SECTION("Perlin sphere")
		{
			mode = PERLIN_SPHERE;
		}
		SECTION("Obj model")
		{
			mode = OBJ_MODEL;
		}
		SECTION("Obj model reader parallel")
		{
			mode = OBJ_MODEL_PARALLEL;
		}
		expected_location = point3{ 0,0,12 };
	}

	render_settings = std::make_unique<renderer_settings>(mode,true);
	render_settings->extra_parse();


	renderMode render_mode;
	Path_type path_type;
	point3 location;
	point3 center;
	double radius, theta;
	int num_seconds, fps;
	render_settings->return_render_mode(render_mode);
	render_settings->return_path_type(path_type);
	render_settings->return_location_param(location);
	render_settings->return_movie_params(num_seconds, fps);
	render_settings->return_circular_params(center, radius, theta);

	REQUIRE(render_mode == expected_render_mode);
	REQUIRE(path_type == expected_path_type);
	REQUIRE_THAT(location, point3Matcher(expected_location, 1e-6));
	REQUIRE_THAT(center, point3Matcher(expected_center, 1e-6));
	REQUIRE_THAT(radius, Catch::Matchers::WithinAbs(expected_radius, 1e-6));
	REQUIRE_THAT(theta, Catch::Matchers::WithinAbs(expected_theta, 1e-6));
	REQUIRE(num_seconds == expected_num_seconds);
	REQUIRE(fps == expected_fps);

}


TEST_CASE("Testing non-default renderer settings")
{
	int mode = -1;
	std::unique_ptr<renderer_settings> rend_settings;


	renderMode expected_render_mode = renderMode::STATIC;
	Path_type expected_path_type = Path_type::NONE;
	point3 expected_location = point3(13.0, 2.0, 3.0);
	point3 expected_center{ 0.0,0.0,0.0 };
	double expected_radius{ 0.0 };
	double expected_theta{ 0.0 };
	std::string expected_file_name = "";
	int expected_fps = 0;
	int expected_num_seconds = 0;

	std::string in;




	SECTION("Test-1")
	{
		in = "renderer -renderer_mode animation -path_type circular -radius 15.0 -theta 90.0 ";
		in += " -file test1.ppm -num_seconds 55 -fps 12";
		expected_render_mode = renderMode::ANIMATION;
		expected_path_type = Path_type::CIRCULAR;
		expected_radius = 15.0;
		expected_theta = 90.0;
		expected_file_name = "test1.ppm";
		expected_num_seconds = 55;
		expected_fps = 12;
	}
	SECTION("Test-2")
	{
		in = "renderer -renderer_mode static -path_type none";
		expected_render_mode = renderMode::STATIC;
		expected_path_type = Path_type::NONE;
	}
	SECTION("Test-3")
	{
		in = "renderer -renderer_mode STATIC -path_type NONE";
		expected_render_mode = renderMode::STATIC;
		expected_path_type = Path_type::NONE;
	}
	SECTION("Test-4")
	{
		in = "renderer -renderer_mode ANIMATION -path_type CIRCULAR";
		in += " -radius 25.5 -theta 180.0 -file test4.ppm -num_seconds 10 -fps 24";
		expected_render_mode = renderMode::ANIMATION;
		expected_path_type = Path_type::CIRCULAR;
		expected_radius = 25.5;
		expected_theta = 180.0;
		expected_file_name = "test4.ppm";
		expected_num_seconds = 10;
		expected_fps = 24;
	}
	SECTION("Test-5")
	{
		in = "renderer -renderer_mode animation -path_type file";
		in += " -file camera_path.txt -num_seconds 20 -fps 30";
		expected_render_mode = renderMode::ANIMATION;
		expected_path_type = Path_type::FILE;
		expected_file_name = "camera_path.txt";
		expected_num_seconds = 20;
		expected_fps = 30;
	}
	SECTION("Test-6")
	{
		in = "renderer -renderer_mode ANIMATION -path_type FILE";
		in += " -file path_data.dat -num_seconds 5 -fps 60";
		expected_render_mode = renderMode::ANIMATION;
		expected_path_type = Path_type::FILE;
		expected_file_name = "path_data.dat";
		expected_num_seconds = 5;
		expected_fps = 60;
	}
	SECTION("Test-7")
	{
		in = "renderer -renderer_mode animation -path_type circular";
		in += " -radius 5.0 -theta 45.0 -num_seconds 3 -fps 15";
		expected_render_mode = renderMode::ANIMATION;
		expected_path_type = Path_type::CIRCULAR;
		expected_radius = 5.0;
		expected_theta = 45.0;
		expected_num_seconds = 3;
		expected_fps = 15;
	}




	rend_settings = std::make_unique<renderer_settings>(mode, true);
	rend_settings->add_cmd(in);
	rend_settings->parse_commands();
	rend_settings->extra_parse();



	renderMode render_mode;
	Path_type path_type;
	point3 location;
	point3 center;
	double radius, theta;
	int num_seconds, fps;
	std::string file_name;
	rend_settings->return_render_mode(render_mode);
	rend_settings->return_path_type(path_type);
	rend_settings->return_location_param(location);
	rend_settings->return_movie_params(num_seconds, fps);
	rend_settings->return_circular_params(center, radius, theta);
	rend_settings->return_file_name(file_name);


	REQUIRE(render_mode == expected_render_mode);
	REQUIRE(path_type == expected_path_type);
	REQUIRE_THAT(location, point3Matcher(expected_location, 1e-6));
	REQUIRE_THAT(center, point3Matcher(expected_center, 1e-6));
	REQUIRE_THAT(radius, Catch::Matchers::WithinAbs(expected_radius, 1e-6));
	REQUIRE_THAT(theta, Catch::Matchers::WithinAbs(expected_theta, 1e-6));
	REQUIRE(num_seconds == expected_num_seconds);
	REQUIRE(fps == expected_fps);
	REQUIRE(file_name == expected_file_name);
}
