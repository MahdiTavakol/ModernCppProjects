#include "test_shared.h"
#include "../Geometry/sphere.h"
#include "../Geometry/mesh.h"
#include "../Geometry/triangle_mesh.h"
#include "../Geometry/quad.h"

TEST_CASE("Testing sorting objects in the hittable list")
{
	// the main hittable_list
	auto world = std::make_unique<hittable_list>();
	// expected result
	auto expectedWorld = std::make_unique<hittable_list>();


	// testing various spheres
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

	
	// Testing various meshes
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
			expectedVs[1] = vs[3];
			expectedVs[2] = vs[1];
			expectedVs[3] = vs[2];
			expectedVs[4] = vs[4];
		}

		SECTION("Mesh-2") {

			// Mesh 0 (tilted rectangle in 3D)
			vs[0] = {
				point3{0.0, 0.0, 0.0},
				point3{4.0, 0.0, 1.0},
				point3{4.0, 1.0, 1.0},
				point3{0.0, 1.0, 0.0}
			};
			// Area ≈ 4.123
		
			// Mesh 1 (unit square shifted)
			vs[1] = {
				point3{1.0, 1.0, 0.0},
				point3{2.0, 1.0, 0.0},
				point3{2.0, 2.0, 0.0},
				point3{1.0, 2.0, 0.0}
			};
			// Area = 1.0

			// Mesh 2 (diamond shape in XY plane)
			vs[2] = {
				point3{0.0, 2.0, 0.0},
				point3{2.0, 0.0, 0.0},
				point3{4.0, 2.0, 0.0},
				point3{2.0, 4.0, 0.0}
			};
			// Area = 8.0

			// Mesh 3 (non-planar skew)
			vs[3] = {
				point3{0.0, 0.0, 0.0},
				point3{2.0, 0.0, 2.0},
				point3{2.0, 2.0, 1.0},
				point3{0.0, 2.0, -1.0}
			};
			// Area ≈ 6.000

			// Mesh 4 (large tilted quad)
			vs[4] = {
				point3{-2.0, -2.0, 0.0},
				point3{2.0, -2.0, 2.0},
				point3{2.0, 2.0, 2.0},
				point3{-2.0, 2.0, 0.0}
			};
			// Area ≈ 16.492

			// expected results (sorted by area)
			expectedVs[0] = vs[1];
			expectedVs[1] = vs[0];
			expectedVs[2] = vs[3];
			expectedVs[3] = vs[2];
			expectedVs[4] = vs[4];
		}

		SECTION("Mesh-3") {

			// Mesh 0 (non-planar twisted quad)
			vs[0] = {
				point3{0.0, 0.0, 0.0},
				point3{2.0, 0.0, 2.0},
				point3{3.0, 3.0, 0.0},
				point3{0.0, 2.0, -2.0}
			};
			// Area ≈ 10.392304845
			
			// Mesh 1 (small rectangle)
			vs[1] = {
				point3{0.0, 0.0, 0.0},
				point3{1.0, 0.0, 0.0},
				point3{1.0, 3.0, 0.0},
				point3{0.0, 3.0, 0.0}
			};
			// Area = 3.0

			// Mesh 2 (tilted parallelogram)
			vs[2] = {
				point3{0.0, 0.0, 0.0},
				point3{3.0, 0.0, 1.0},
				point3{4.0, 2.0, 1.0},
				point3{1.0, 2.0, 0.0}
			};
			// Area ≈ 6.403124237

			// Mesh 3 (large irregular 3D quad)
			vs[3] = {
				point3{-3.0, -1.0, 0.0},
				point3{4.0, -2.0, 3.0},
				point3{3.0, 3.0, 1.0},
				point3{-2.0, 2.0, -1.0}
			};
			// Area ≈ 27.593990809

			// Mesh 4 (skewed planar quad)
			vs[4] = {
				point3{0.0, 0.0, 0.0},
				point3{3.0, 1.0, 0.0},
				point3{2.0, 4.0, 0.0},
				point3{-1.0, 3.0, 0.0}
			};
			// Area ≈ 10.0

			// expected results (sorted by area)
			expectedVs[0] = vs[1];
			expectedVs[1] = vs[2];
			expectedVs[2] = vs[4];
			expectedVs[3] = vs[0];
			expectedVs[4] = vs[3];
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


	// Testing various traingular meshes
	SECTION("Triangle Mesh")
	{
		std::array<point3, 3> vs[5];
		std::array<point3, 3> expectedVs[5];
		std::array<point3, 3> zero3Point3 =
		{ point3(),point3(),point3()};

		std::array<point3, 3> vts[5] = {
			zero3Point3,
			zero3Point3,
			zero3Point3,
			zero3Point3,
			zero3Point3
		};
		std::array<point3, 3> vns[5] = {
			zero3Point3,
			zero3Point3,
			zero3Point3,
			zero3Point3,
			zero3Point3
		};

		SECTION("TriangleMesh-1") {

			// Triangle 0
			vs[0] = {
				point3{0.0, 0.0, 0.0},
				point3{2.0, 0.0, 0.0},
				point3{0.0, 2.0, 0.0}
			};
			// Area = 2.0

			// Triangle 1
			vs[1] = {
				point3{0.0, 0.0, 0.0},
				point3{3.0, 0.0, 0.0},
				point3{0.0, 4.0, 0.0}
			};
			// Area = 6.0

			// Triangle 2
			vs[2] = {
				point3{0.0, 0.0, 0.0},
				point3{1.0, 0.0, 1.0},
				point3{0.0, 2.0, 0.0}
			};
			// Area = 1.4142135624

			// Triangle 3
			vs[3] = {
				point3{1.0, 1.0, 0.0},
				point3{4.0, 1.0, 0.0},
				point3{1.0, 5.0, 0.0}
			};
			// Area = 6.0

			// Triangle 4
			vs[4] = {
				point3{0.0, 0.0, 0.0},
				point3{2.0, 1.0, 0.0},
				point3{1.0, 3.0, 0.0}
			};
			// Area = 2.5

			// expected results (sorted by area)
			expectedVs[0] = vs[2]; // 1.4142
			expectedVs[1] = vs[0]; // 2.0
			expectedVs[2] = vs[4]; // 2.5
			expectedVs[3] = vs[1]; // 6.0
			expectedVs[4] = vs[3]; // 6.0
		}

		SECTION("TriangleMesh-2") {

			// Triangle 0
			vs[0] = {
				point3{0.0, 0.0, 0.0},
				point3{1.0, 0.0, 0.0},
				point3{0.0, 1.0, 0.0}
			};
			// Area = 0.5

			// Triangle 1
			vs[1] = {
				point3{0.0, 0.0, 0.0},
				point3{2.0, 0.0, 0.0},
				point3{0.0, 3.0, 0.0}
			};
			// Area = 3.0

			// Triangle 2
			vs[2] = {
				point3{0.0, 0.0, 1.0},
				point3{2.0, 0.0, 1.0},
				point3{0.0, 2.0, 3.0}
			};
			// Area = 2.8284271247

			// Triangle 3
			vs[3] = {
				point3{-1.0, 0.0, 0.0},
				point3{2.2, 0.0, 0.0},
				point3{0.0, 2.0, 0.0}
			};
			// Area = 3.2

			// Triangle 4
			vs[4] = {
				point3{1.0, 1.0, 1.0},
				point3{4.0, 1.0, 2.0},
				point3{2.0, 5.0, 1.0}
			};
			// Area = 6.3442887702

			// expected results (sorted by area)
			expectedVs[0] = vs[0]; // 0.5
			expectedVs[1] = vs[2]; // 2.8284271247
			expectedVs[2] = vs[1]; // 3.0
			expectedVs[3] = vs[3]; // 3.2
			expectedVs[4] = vs[4]; // 6.3443
		}

	
		SECTION("TriangleMesh-3") {

			// Triangle 0
			vs[0] = {
				point3{0.0, 0.0, 0.0},
				point3{4.0, 0.0, 0.0},
				point3{0.0, 1.0, 0.0}
			};
			// Area = 2.0

			// Triangle 1
			vs[1] = {
				point3{0.0, 0.0, 0.0},
				point3{0.0, 3.0, 0.0},
				point3{4.0, 0.0, 0.0}
			};
			// Area = 6.0

			// Triangle 2
			vs[2] = {
				point3{0.0, 0.0, 0.0},
				point3{2.0, 2.0, 0.0},
				point3{0.0, 2.0, 2.0}
			};
			// Area = 3.4641016151

			// Triangle 3
			vs[3] = {
				point3{-1.0, -1.0, 0.0},
				point3{3.0, -1.0, 1.0},
				point3{1.0, 2.0, 0.0}
			};
			// Area = 6.3442887702

			// Triangle 4
			vs[4] = {
				point3{1.0, 0.0, 0.0},
				point3{5.0, 0.0, 0.0},
				point3{3.0, 4.0, 0.0}
			};
			// Area = 8.0

			// expected results (sorted by area)
			expectedVs[0] = vs[0]; // 2.0
			expectedVs[1] = vs[2]; // 3.4641
			expectedVs[2] = vs[1]; // 6.0
			expectedVs[3] = vs[3]; // 6.3443
			expectedVs[4] = vs[4]; // 8.0
		}
		


		for (int i = 0; i < 5; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<hittable> msh =
				std::make_unique<triangle_mesh>(vs[i], vts[i], vns[i], std::move(mat));
			world->add(std::move(msh));
		}
		for (int i = 0; i < 5; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<hittable> msh =
				std::make_unique<triangle_mesh>(expectedVs[i], vts[i], vns[i], std::move(mat));
			expectedWorld->add(std::move(msh));
		}

		// testing quadilaterals
		/*
		SECTION("Quads")
		{
			point3 Q{ 0.0,0.0,0.0 };
			vec3 u[5];
			vec3 v[5];
			vec3 expectedU[5];
			vec3 expectedV[5];

			SECTION("Quad-1") {

				// Quad 0 (unit square)
				u[0] = vec3{ 1.0, 0.0, 0.0 };
				v[0] = vec3{ 0.0, 1.0, 0.0 };
				// Area = 1.0

				// Quad 1 (scaled rectangle)
				u[1] = vec3{ 3.0, 0.0, 0.0 };
				v[1] = vec3{ 0.0, 2.0, 0.0 };
				// Area = 6.0

				// Quad 2 (tilted in Z)
				u[2] = vec3{ 2.0, 0.0, 1.0 };
				v[2] = vec3{ 0.0, 2.0, 0.0 };
				// |u × v| = sqrt( (2*0 - 1*2)^2 + ... ) = sqrt(4 + 0 + 16) = sqrt(20)
				// Area ≈ 4.472135955

				// Quad 3 (skewed)
				u[3] = vec3{ 1.0, 2.0, 0.0 };
				v[3] = vec3{ 2.0, 1.0, 0.0 };
				// cross = (0,0,-3) → Area = 3.0

				// Quad 4 (3D skew)
				u[4] = vec3{ 1.0, 2.0, 3.0 };
				v[4] = vec3{ 2.0, 1.0, 1.0 };
				// cross = (-1,5,-3) → |.| = sqrt(35)
				// Area ≈ 5.916079783

				// expected sorted by area
				expectedU[0] = u[0];
				expectedU[1] = u[3];
				expectedU[2] = u[2];
				expectedU[3] = u[4];
				expectedU[4] = u[1];
				expectedV[0] = v[0];
				expectedV[1] = v[3];
				expectedV[2] = v[2];
				expectedV[3] = v[4];
				expectedV[4] = v[1];
			}


			for (int i = 0; i < 5; i++)
			{
				std::unique_ptr<material> mat = std::make_unique<fake_material>();
				std::unique_ptr<hittable> msh =
					std::make_unique<quad>(Q,u[i], v[i], std::move(mat));
				world->add(std::move(msh));
			}
			for (int i = 0; i < 5; i++)
			{
				std::unique_ptr<material> mat = std::make_unique<fake_material>();
				std::unique_ptr<hittable> msh =
					std::make_unique<quad>(Q,expectedU[i], expectedV[i], std::move(mat));
				expectedWorld->add(std::move(msh));
			}

		}
		*/

	}


	


	// sorting the objects in the hittable_list
	world->sort();

	// comparing the results
	REQUIRE_THAT(world.get(), hittableListMatcher(expectedWorld.get(), 1e-6));
}