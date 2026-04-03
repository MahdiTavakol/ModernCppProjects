#include "test_shared.h"
#include "../Geometry/triangle_mesh.h"

TEST_CASE("Testing the area of the triangle_meshes")
{
	std::array<point3, 3> vs;
	std::array<point3, 3> zero3Point3 =
	{ point3(),point3(),point3() };

	std::array<point3, 3> vts = zero3Point3;
	std::array<point3, 3> vns = zero3Point3;

	double expectedArea = 0.0;

	SECTION("triangle_mesh-1")
	{
		vs = {
			point3{ 0.0, 0.0, 0.0 },
			point3{ 2.0, 0.0, 0.0 },
			point3{ 0.0, 2.0, 0.0 }
		};
		expectedArea = 2.0;
	}

	SECTION("triangle_mesh-1")
	{
		vs = {
			point3{ 0.0, 0.0, 0.0 },
			point3{ 2.0, 0.0, 0.0 },
			point3{ 0.0, 2.0, 0.0 }
		};
		expectedArea = 2.0;
	}

	SECTION("triangle_mesh-1")
	{
		vs = {
			point3{ 0.0, 0.0, 0.0 },
			point3{ 2.0, 0.0, 0.0 },
			point3{ 0.0, 2.0, 0.0 }
		};
		expectedArea = 2.0;
	}

	SECTION("triangle_mesh-1")
	{
		vs = 
		{
			point3{ 0.0, 0.0, 0.0 },
			point3{ 2.0, 0.0, 0.0 },
			point3{ 0.0, 2.0, 0.0 }
		};
		expectedArea = 2.0;
	}

	SECTION("triangle_mesh-2")
	{
		vs = 
		{
			point3{0.0, 0.0, 0.0},
			point3{3.0, 0.0, 0.0},
			point3{0.0, 4.0, 0.0}
		};
		expectedArea = 6.0;
	}

	SECTION("triangle_mesh-3")
	{
		vs = 
		{
			point3{ 0.0, 0.0, 0.0 },
			point3{ 1.0, 0.0, 1.0 },
			point3{ 0.0, 2.0, 0.0 }
		};
		expectedArea = 1.4142135624;
	}

	SECTION("triangle_mesh-4")
	{
		vs = 
		{
			point3{1.0, 1.0, 0.0},
			point3{4.0, 1.0, 0.0},
			point3{1.0, 5.0, 0.0}
		};
		expectedArea = 6.0;
	}

	SECTION("triangle_mesh-5")
	{
		vs = 
		{
			point3{0.0, 0.0, 0.0},
			point3{2.0, 1.0, 0.0},
			point3{1.0, 3.0, 0.0}
		};
		expectedArea = 2.5;
	}

	SECTION("triangle_mesh-6")
	{
		vs = 
		{
			point3{0.0, 0.0, 0.0},
			point3{1.0, 0.0, 0.0},
			point3{0.0, 1.0, 0.0}
		};
		expectedArea = 0.5;
	}

	SECTION("triangle_mesh-7")
	{
		vs =
		{
			point3{0.0, 0.0, 1.0},
			point3{2.0, 0.0, 1.0},
			point3{0.0, 2.0, 3.0}
		};
		expectedArea = 2.8284271247;
	}

	SECTION("triangle_mesh-8")
	{
		vs =
		{
			point3{1.0, 1.0, 1.0},
			point3{4.0, 1.0, 2.0},
			point3{2.0, 5.0, 1.0}
		};
		expectedArea = 6.3442887702;
	}

	std::unique_ptr<material> mat =
		std::make_unique<fake_material>();
	auto t_mesh = std::make_unique<triangle_mesh>(vs, vts, vns, std::move(mat));

	double area = t_mesh->get_area();

	REQUIRE_THAT(area, Catch::Matchers::WithinAbs(expectedArea, 1e-6));
}