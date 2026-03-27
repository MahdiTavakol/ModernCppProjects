#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include "../Input/input.h"
#include "../Geometry/triangle_mesh.h"
#include "test_shared.h"
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


TEST_CASE("Testing the compare function of the vec3")
{
	vec3 a{ 1.0, 2.0, 3.0 };
	vec3 b{ 1.0, 2.0, 3.0 };
	vec3 c{ 1.0, 2.0, 4.0 };
	vec3 d{ 1.0, 2.0, 2.0 };
	vec3 e{ 1.0, 1.0, 3.0 };
	vec3 f{ 1.0, 4.0, 3.0 };
	vec3 g{ 0.0, 2.0, 3.0 };
	vec3 h{ 2.0, 2.0, 3.0 };

	REQUIRE(a == b);
	REQUIRE(a < c);
	REQUIRE(a > d);
	REQUIRE(a > e);
	REQUIRE(a < f);
	REQUIRE(a > g);
	REQUIRE(a < h);
}

TEST_CASE("Testing the x(), y(), z() and [] functions of the vec3")
{
	vec3 a{ 2.0,3.5,4.8 };

	double x = a.x();
	double y = a.y();
	double z = a.z();

	REQUIRE_THAT(x, Catch::Matchers::WithinAbs(2.0, 1e-6));
	REQUIRE_THAT(y, Catch::Matchers::WithinAbs(3.5, 1e-6));
	REQUIRE_THAT(z, Catch::Matchers::WithinAbs(4.8, 1e-6));
	REQUIRE_THAT(a[0], Catch::Matchers::WithinAbs(2.0, 1e-6));
	REQUIRE_THAT(a[1], Catch::Matchers::WithinAbs(3.5, 1e-6));
	REQUIRE_THAT(a[2], Catch::Matchers::WithinAbs(4.8, 1e-6));
}

TEST_CASE("Testing the vec3 functions")
{
	// inputs
	vec3 a{ 1.5,8.2,6.5 };
	vec3 b = a;
	vec3 c = a;
	vec3 d = a;


	// operations
	vec3 e = -a;
	b *= 3.2;
	c += vec3{ 2.2,3.6,8.5 };
	d /= 2.0;

	// functions
	double length = a.length();
	double length_sq = a.length_squared();

	// expected results
	vec3 expected_e = { -1.5,-8.2,-6.5 };
	vec3 expected_b = { 4.8,26.24,20.8 };
	vec3 expected_c = { 3.7,11.8,15.0 };
	vec3 expected_d = { 0.75,4.1,3.25 };
	double expected_length = 10.57071426158138;
	double expected_length_sq = 111.74;


	// checking results
	REQUIRE_THAT(e, vec3Matcher(expected_e, 1e-6));
	REQUIRE_THAT(b, vec3Matcher(expected_b, 1e-6));
	REQUIRE_THAT(c, vec3Matcher(expected_c, 1e-6));
	REQUIRE_THAT(d, vec3Matcher(expected_d, 1e-6));
	REQUIRE_THAT(length, Catch::Matchers::WithinAbs(expected_length, 1e-6));
	REQUIRE_THAT(length_sq, Catch::Matchers::WithinAbs(expected_length_sq, 1e-6));
}


TEST_CASE("Testing colorArrayMatcher")
{
	color_array c_array_1;
	color_array c_array_2;
	c_array_1.reset_size(3, 3);
	c_array_2.reset_size(3, 3);
	

	c_array_1(0, 0) = color_data{ 1.0, 2.0, 3.0 };
	c_array_2(0, 0) = color_data{ 1.0, 2.0, 3.0 };

	c_array_1(0, 1) = color_data{ 0.42, 0.73, 0.15 };
	c_array_2(0, 1) = color_data{ 0.42, 0.73, 0.15 };

	c_array_1(0, 2) = color_data{ 0.91, 0.12, 0.58 };
	c_array_2(0, 2) = color_data{ 0.91, 0.12, 0.58 };

	c_array_1(1, 0) = color_data{ 0.33, 0.67, 0.25 };
	c_array_2(1, 0) = color_data{ 0.33, 0.67, 0.25 };

	c_array_1(1, 1) = color_data{ 0.88, 0.44, 0.19 };
	c_array_2(1, 1) = color_data{ 0.88, 0.44, 0.19 };

	c_array_1(1, 2) = color_data{ 0.56, 0.81, 0.36 };
	c_array_2(1, 2) = color_data{ 0.56, 0.81, 0.36 };

	c_array_1(2, 0) = color_data{ 0.14, 0.95, 0.62 };
	c_array_2(2, 0) = color_data{ 0.14, 0.95, 0.62 };

	c_array_1(2, 1) = color_data{ 0.77, 0.21, 0.49 };
	c_array_2(2, 1) = color_data{ 0.77, 0.21, 0.49 };

	c_array_1(2, 2) = color_data{ 0.65, 0.39, 0.84 };
	c_array_2(2, 2) = color_data{ 0.65, 0.39, 0.84 };

	color_array* c_array_1_ptr = &c_array_1;
	color_array* c_array_2_ptr = &c_array_2;

	REQUIRE_THAT(c_array_1_ptr, colorArrayMatcher(c_array_2_ptr, 1e-6));

}



