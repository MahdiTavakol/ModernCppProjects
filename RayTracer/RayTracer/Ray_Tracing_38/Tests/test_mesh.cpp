#include "test_shared.h"
#include "../Geometry/mesh.h"

TEST_CASE("Testing the area calculated by the mesh object")
{
	std::array<point3, 4> vs, vts, vns;
	std::array<point3, 4> zero4Array =
	{
		point3{},point3{},point3{},point3{}
	};
	vts = zero4Array;
	vns = zero4Array;

	double expectedArea;

	SECTION("Mesh-1")
	{
		vs =
		{
			point3{0.0, 0.0, 0.0},
			point3{2.0, 0.0, 0.0},
			point3{2.0, 2.0, 0.0},
			point3{0.0, 2.0, 0.0}
		};

		expectedArea = 4.0;
	}

	SECTION("Mesh-2")
	{
		// Mesh 1 (rectangle tilted in Z)
		vs = {
			point3{0.0, 0.0, 0.0},
			point3{3.0, 0.0, 0.0},
			point3{3.0, 2.0, 1.0},
			point3{0.0, 2.0, 1.0}
		};

		expectedArea = 6.7082039325;
	}

	SECTION("Mesh-3") {
		// Mesh 2 (irregular quad)
		vs = {
			point3{1.0, 0.0, 0.0},
			point3{4.0, 1.0, 0.0},
			point3{3.0, 4.0, 0.0},
			point3{0.0, 3.0, 0.0}
		};
		
		expectedArea = 10.0;
	}

	SECTION("Mesh-4") {
		// Mesh 3 (non-planar quad)
		vs = {
			point3{0.0, 0.0, 0.0},
			point3{2.0, 0.0, 1.0},
			point3{2.0, 2.0, 0.0},
			point3{0.0, 2.0, -1.0}
		};
		expectedArea = 4.8989794856;
	}

	SECTION("Mesh-5") {

		// Mesh 4 (larger tilted quad)
		vs = {
			point3{-1.0, -1.0, 0.0},
			point3{3.0, -1.0, 2.0},
			point3{2.0, 3.0, 1.0},
			point3{-2.0, 2.0, -1.0}
		};
		expectedArea = 16.4498322129;
	}


	std::unique_ptr<material> mat = std::make_unique<fake_material>();

	std::unique_ptr<mesh> obj = std::make_unique<mesh>(vs, vts, vns,std::move(mat));

	double area = 0.0;

	area = obj->get_area();

	REQUIRE_THAT(area, Catch::Matchers::WithinAbs(expectedArea,1e-6));

}