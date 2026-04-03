#include "test_shared.h"
#include "../Geometry/quad.h"

TEST_CASE("Testing the area of quads")
{
	point3 Q{ 0.0,0.0,0.0 };
	vec3 u, v;
	double expectedArea = 0.0;

	SECTION("Quad-1")
	{
		u = vec3{ 1.0, 0.0, 0.0 };
		v = vec3{ 0.0, 1.0, 0.0 };
		expectedArea = 1.0;
	}

	SECTION("Quad-2")
	{
		u = vec3{ 3.0, 0.0, 0.0 };
		v = vec3{ 0.0, 2.0, 0.0 };
		expectedArea = 6.0;
	}

	SECTION("Quad-3")
	{
		u = vec3{ 2.0, 0.0, 1.0 };
		v = vec3{ 0.0, 2.0, 0.0 };
		expectedArea = 4.472135955;
	}

	SECTION("Quad-4")
	{
		u = vec3{ 1.0, 2.0, 0.0 };
		v = vec3{ 2.0, 1.0, 0.0 };
		expectedArea = 3.0;
	}

	SECTION("Quad-5")
	{
		u = vec3{ 1.0, 2.0, 3.0 };
		v = vec3{ 2.0, 1.0, 1.0 };
		expectedArea = 5.916079783;
	}

	std::unique_ptr<material> mat = std::make_unique<fake_material>();
	auto obj =  std::make_unique<quad>(Q, u, v, std::move(mat));


	double area = obj->get_area();

	REQUIRE_THAT(area, Catch::Matchers::WithinAbs(expectedArea, 1e-6));

}

TEST_CASE("Testing the compare function of the quads")
{
	point3 Q1{ 0.0,1.0,2.0 };
	point3 Q2{ 1.0,3.0,5.0 };
	vec3 u1, v1;
	vec3 u2, v2;
	std::unique_ptr<quad> qd1, qd2;
	
	SECTION("Equality") {
		SECTION("Test-1")
		{
			u1 = vec3{ 1.0, 2.0, 3.0 };
			v1 = vec3{ 2.0, 1.0, 1.0 };
		}
		SECTION("Test-2")
		{
			u1 = vec3{ 2.0, 0.0, 1.0 };
			v1 = vec3{ 0.0, 2.0, 0.0 };
		}
		SECTION("Test-3")
		{
			u1 = vec3{ 1.0, 0.0, 0.0 };
			v1 = vec3{ 0.0, 1.0, 0.0 };
		}
		SECTION("Test-4")
		{
			u1 = vec3{ 6.0, 0.5, 0.2 };
			v1 = vec3{ 0.4, 3.0, 0.7 };
		}
		u2 = u1;
		v2 = v1;
		std::unique_ptr<material> mat1 = std::make_unique<fake_material>();
		std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
		qd1 = std::make_unique<quad>(Q1, u1, v1, std::move(mat1));
		qd2 = std::make_unique<quad>(Q2, u2, v2, std::move(mat2));
		REQUIRE(!qd1->compare(qd2.get(), 1e-6));
	}
	SECTION("Inequality") {
		SECTION("Test-1")
		{
			u1 = vec3{ 1.0, 2.0, 3.0 };
			v1 = vec3{ 2.0, 1.0, 1.0 };
		}
		SECTION("Test-2")
		{
			u1 = vec3{ 2.0, 0.0, 1.0 };
			v1 = vec3{ 0.0, 2.0, 0.0 };
		}
		SECTION("Test-3")
		{
			u1 = vec3{ 1.0, 0.0, 0.0 };
			v1 = vec3{ 0.0, 1.0, 0.0 };
		}
		SECTION("Test-4")
		{
			u1 = vec3{ 6.0, 0.5, 0.2 };
			v1 = vec3{ 0.4, 3.0, 0.7 };
		}
		u2 = v1;
		v2 = u1;
		std::unique_ptr<material> mat1 = std::make_unique<fake_material>();
		std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
		qd1 = std::make_unique<quad>(Q1, u1, v1, std::move(mat1));
		qd2 = std::make_unique<quad>(Q2, u2, v2, std::move(mat2));
		REQUIRE(qd1->compare(qd2.get(), 1e-6));
	}
}