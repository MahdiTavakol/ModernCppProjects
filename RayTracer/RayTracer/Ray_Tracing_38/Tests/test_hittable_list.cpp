#include "test_shared.h"
#include "../Geometry/sphere.h"
#include "../Geometry/mesh.h"

TEST_CASE("Testing sorting objects in the hittable list")
{
	// the main hittable_list
	auto world = std::make_unique<hittable_list>();
	// expected result
	auto expectedWorld = std::make_unique<hittable_list>();

	SECTION("Spheres")
	{
		struct sphereInfoType {
			point3 center;
			double radius;
		};

		point3 centers[4];
		double radius[4];
		point3 sortedCenters[4];
		double sortedRadius[4];

		SECTION("Spheres-1") {
			centers[0] = point3{0.0,2.0,-3.5};
			centers[1] = point3{12.0, 15.0, 30.0};
			centers[2] = point3{-2.0,-8.0,9.0};
			centers[3] = point3{0.8,-9.0,4.5};
			radius[0] = 12.0;
			radius[1] = 0.8;
			radius[2] = 16.0;
			radius[3] = 5.0;

			sortedCenters[0] = point3{ 12.0, 15.0, 30.0 };
			sortedCenters[1] = point3{ 0.8,-9.0,4.5 };
			sortedCenters[2] = point3{ 0.0,2.0,-3.5 };
			sortedCenters[3] = point3{ -2.0,-8.0,9.0 };
			sortedRadius[0] = 0.8;
			sortedRadius[1] = 5.0;
			sortedRadius[2] = 12.0;
			sortedRadius[3] = 16.0;

		}

		SECTION("Spheres-2")
		{
			centers[0] = point3{ 5.0, -3.0, 2.0 };
			centers[1] = point3{ -10.0, 4.0, 1.0 };
			centers[2] = point3{ 7.5, 8.0, -6.0 };
			centers[3] = point3{ 0.0, 0.0, 0.0 };

			radius[0] = 3.2;
			radius[1] = 10.5;
			radius[2] = 1.1;
			radius[3] = 6.0;

			sortedCenters[0] = point3{ 7.5, 8.0, -6.0 };
			sortedCenters[1] = point3{ 5.0, -3.0, 2.0 };
			sortedCenters[2] = point3{ 0.0, 0.0, 0.0 };
			sortedCenters[3] = point3{ -10.0, 4.0, 1.0 };

			sortedRadius[0] = 1.1;
			sortedRadius[1] = 3.2;
			sortedRadius[2] = 6.0;
			sortedRadius[3] = 10.5;
		}

		SECTION("Spheres-3")
		{
			centers[0] = point3{ 5.0, -3.0, 2.0 };
			centers[1] = point3{ -10.0, 4.0, 1.0 };
			centers[2] = point3{ 7.5, 8.0, -6.0 };
			centers[3] = point3{ 0.0, 0.0, 0.0 };

			radius[0] = 3.2;
			radius[1] = 10.5;
			radius[2] = 1.1;
			radius[3] = 6.0;

			sortedCenters[0] = point3{ 7.5, 8.0, -6.0 };
			sortedCenters[1] = point3{ 5.0, -3.0, 2.0 };
			sortedCenters[2] = point3{ 0.0, 0.0, 0.0 };
			sortedCenters[3] = point3{ -10.0, 4.0, 1.0 };

			sortedRadius[0] = 1.1;
			sortedRadius[1] = 3.2;
			sortedRadius[2] = 6.0;
			sortedRadius[3] = 10.5;
		}

		SECTION("Spheres-4")
		{
			centers[0] = point3{ 1.0, 1.0, 1.0 };
			centers[1] = point3{ -2.0, -2.0, -2.0 };
			centers[2] = point3{ 4.5, -7.0, 3.3 };
			centers[3] = point3{ 6.0, 2.0, -5.0 };

			radius[0] = 7.7;
			radius[1] = 0.5;
			radius[2] = 12.3;
			radius[3] = 3.0;

			sortedCenters[0] = point3{ -2.0, -2.0, -2.0 };
			sortedCenters[1] = point3{ 6.0, 2.0, -5.0 };
			sortedCenters[2] = point3{ 1.0, 1.0, 1.0 };
			sortedCenters[3] = point3{ 4.5, -7.0, 3.3 };

			sortedRadius[0] = 0.5;
			sortedRadius[1] = 3.0;
			sortedRadius[2] = 7.7;
			sortedRadius[3] = 12.3;
		}

		sphereInfoType
			sphereInfo[4] =
		{
			{centers[0],radius[0]},
			{centers[1],radius[1]},
			{centers[2],radius[2]},
			{centers[3],radius[3]}
		};

		sphereInfoType
			sortedSphereInfo[4] = {
			{sortedCenters[0],sortedRadius[0]},
			{sortedCenters[1],sortedRadius[1]},
			{sortedCenters[2],sortedRadius[2]},
			{sortedCenters[3],sortedRadius[3]}
		};


		for (auto& info : sphereInfo)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<hittable> sphr = std::make_unique<sphere>(info.center, info.radius, std::move(mat));
			world->add(std::move(sphr));
		}
		for (auto& info : sortedSphereInfo)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<hittable> sphr = std::make_unique<sphere>(info.center, info.radius, std::move(mat));
			expectedWorld->add(std::move(sphr));
		}
	}

	SECTION("Mesh")
	{
		std::array<point3, 4> vs[5];
		std::array<point3, 4> expectedVs[5];
		std::array<point3, 4> zero4Point3 =
		{ point3(),point3(),point3(),point3() };

		std::array<point3, 4> vts[5] = {
			zero4Point3,
			zero4Point3,
			zero4Point3,
			zero4Point3,
			zero4Point3
		};
		std::array<point3, 4> vns[5] = {
			zero4Point3,
			zero4Point3,
			zero4Point3,
			zero4Point3,
			zero4Point3
		};



		SECTION("Mesh-1") {

			// Mesh 0 (square in XY plane)
			vs[0] = {
				point3{0.0, 0.0, 0.0},
				point3{2.0, 0.0, 0.0},
				point3{2.0, 2.0, 0.0},
				point3{0.0, 2.0, 0.0}
			};
			// Area = 4.0

			// Mesh 1 (rectangle tilted in Z)
			vs[1] = {
				point3{0.0, 0.0, 0.0},
				point3{3.0, 0.0, 0.0},
				point3{3.0, 2.0, 1.0},
				point3{0.0, 2.0, 1.0}
			};
			// Area ≈ 6.708

			// Mesh 2 (irregular quad)
			vs[2] = {
				point3{1.0, 0.0, 0.0},
				point3{4.0, 1.0, 0.0},
				point3{3.0, 4.0, 0.0},
				point3{0.0, 3.0, 0.0}
			};
			// Area ≈ 10.0

			// Mesh 3 (non-planar quad)
			vs[3] = {
				point3{0.0, 0.0, 0.0},
				point3{2.0, 0.0, 1.0},
				point3{2.0, 2.0, 0.0},
				point3{0.0, 2.0, -1.0}
			};
			// Area ≈ 4.899

			// Mesh 4 (larger tilted quad)
			vs[4] = {
				point3{-1.0, -1.0, 0.0},
				point3{3.0, -1.0, 2.0},
				point3{2.0, 3.0, 1.0},
				point3{-2.0, 2.0, -1.0}
			};
			// Area ≈ 13.416

			// expected results
			expectedVs[0] = vs[0];
			expectedVs[0] = vs[3];
			expectedVs[0] = vs[1];
			expectedVs[0] = vs[2];
			expectedVs[0] = vs[4];
		}

		for (int i = 0; i < 5; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<hittable> msh = 
				std::make_unique<mesh>(vs[i],vts[i],vns[i], std::move(mat));
			world->add(std::move(msh));
		}
		for (int i = 0; i < 5; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<hittable> msh =
				std::make_unique<mesh>(expectedVs[i], vts[i], vns[i], std::move(mat));
			expectedWorld->add(std::move(msh));
		}

	}


	// sorting the objects in the hittable_list
	world->sort();

	// comparing the results
	REQUIRE_THAT(world.get(), hittableListMatcher(expectedWorld.get(), 1e-6));
}