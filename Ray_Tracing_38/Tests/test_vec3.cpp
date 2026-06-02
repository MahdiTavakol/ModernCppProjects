#include "catch_amalgamated.hpp"
#include "test_shared.h"


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