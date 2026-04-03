#include "test_shared.h"
#include "../Geometry/sphere.h"
#include "../Geometry/mesh.h"
#include "../Geometry/triangle_mesh.h"
#include "../Geometry/quad.h"

#include <algorithm>
#include <random>
#include <numeric>

TEST_CASE("Testing sorting similar objects in the hittable list")
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
		{ point3(),point3(),point3() };

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

	}
		// testing quadilaterals

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
			expectedU[0] = u[0]; // 1.0
			expectedU[1] = u[3]; // 3.0
			expectedU[2] = u[2]; // 4.472135955
			expectedU[3] = u[4]; // 5.916079783
			expectedU[4] = u[1]; // 6.0
			expectedV[0] = v[0];
			expectedV[1] = v[3];
			expectedV[2] = v[2];
			expectedV[3] = v[4];
			expectedV[4] = v[1];
		}

		SECTION("Quad-2") {

			// Quad 0 (small square)
			u[0] = vec3{ 0.5, 0.0, 0.0 };
			v[0] = vec3{ 0.0, 0.5, 0.0 };
			// Area = 0.25

			// Quad 1 (rectangle)
			u[1] = vec3{ 4.0, 0.0, 0.0 };
			v[1] = vec3{ 0.0, 3.0, 0.0 };
			// Area = 12.0

			// Quad 2 (tilted in 3D)
			u[2] = vec3{ 2.0, 1.0, 0.0 };
			v[2] = vec3{ 0.0, 2.0, 2.0 };
			// cross = (2, -4, 4) → |.| = sqrt(36)
			// Area = 6.0

			// Quad 3 (skewed planar)
			u[3] = vec3{ 2.0, 1.0, 0.0 };
			v[3] = vec3{ 1.0, 2.0, 0.0 };
			// cross = (0,0,3) → Area = 3.0

			// Quad 4 (3D skew)
			u[4] = vec3{ 3.0, 1.0, 2.0 };
			v[4] = vec3{ 1.0, 4.0, 2.0 };
			// cross = (-6, -4, 11) → |.| = sqrt(173)
			// Area ≈ 13.15294644

			// expected sorted by area
			expectedU[0] = u[0]; // 0.25
			expectedU[1] = u[3]; // 3.0
			expectedU[2] = u[2]; // 6.0
			expectedU[3] = u[1]; // 12.0
			expectedU[4] = u[4]; // 13.1529

			expectedV[0] = v[0];
			expectedV[1] = v[3];
			expectedV[2] = v[2];
			expectedV[3] = v[1];
			expectedV[4] = v[4];
		}

		SECTION("Quad-3") {

			// Quad 0 (XY plane)
			u[0] = vec3{ 2.0, 0.0, 0.0 };
			v[0] = vec3{ 0.0, 5.0, 0.0 };
			// Area = 10.0

			// Quad 1 (XZ plane)
			u[1] = vec3{ 3.0, 0.0, 0.0 };
			v[1] = vec3{ 0.0, 0.0, 4.0 };
			// Area = 12.0

			// Quad 2 (YZ plane)
			u[2] = vec3{ 0.0, 2.0, 0.0 };
			v[2] = vec3{ 0.0, 0.0, 3.0 };
			// Area = 6.0

			// Quad 3 (3D skew)
			u[3] = vec3{ 2.0, 1.0, 1.0 };
			v[3] = vec3{ 1.0, 3.0, 2.0 };
			// cross = (-1, -3, 5) → |.| = sqrt(35)
			// Area ≈ 5.916079783

			// Quad 4 (another 3D skew)
			u[4] = vec3{ 1.0, 2.0, 2.0 };
			v[4] = vec3{ 2.0, 1.0, 3.0 };
			// cross = (4, 1, -3) → |.| = sqrt(26)
			// Area ≈ 5.099019514

			// expected sorted by area
			expectedU[0] = u[4]; // 5.0990
			expectedU[1] = u[3]; // 5.9161
			expectedU[2] = u[2]; // 6.0
			expectedU[3] = u[0]; // 10.0
			expectedU[4] = u[1]; // 12.0

			expectedV[0] = v[4];
			expectedV[1] = v[3];
			expectedV[2] = v[2];
			expectedV[3] = v[0];
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



	// sorting the objects in the hittable_list
	world->sort();

	// comparing the results
	REQUIRE_THAT(world.get(), hittableListMatcher(expectedWorld.get(), 1e-6));
}

TEST_CASE("Sorting different shapes in the hittable_list")
{

	// the main hittable_list
	auto world = std::make_unique<hittable_list>();
	// expected result
	auto expectedWorld = std::make_unique<hittable_list>();
	// number of data
	int nData = 12;

	// random positions
	std::random_device rd;
	std::mt19937_64 gen{ rd() };
	// vector from 0 to 12
	std::vector<int> indx;
	indx.resize(nData);
	std::iota(indx.begin(), indx.end(), 0);
	// shuffling to random positions
	std::shuffle(indx.begin(), indx.end(), gen);
	// current position
	int pos = 0;
	// resizing the world
	world->resize(nData);

	SECTION("Case-1")
	{
		// meshes
		std::array<point3, 4> vm[3];
		std::array<point3, 4> expectedVm[3];
		std::array<point3, 4> vtm[3] =
		{
			{point3{},point3{},point3{},point3{}},
			{point3{},point3{},point3{},point3{}},
			{point3{},point3{},point3{},point3{}}
		};
		std::array<point3, 4> vnm[3] =
		{
			{point3{},point3{},point3{},point3{}},
			{point3{},point3{},point3{},point3{}},
			{point3{},point3{},point3{},point3{}}
		};


		// Mesh 0 (non-planar twisted quad)
		vm[0] = {
			point3{0.0, 0.0, 0.0},
			point3{2.0, 0.0, 2.0},
			point3{3.0, 3.0, 0.0},
			point3{0.0, 2.0, -2.0}
		};
		// Area ≈ 10.392304845

		// Mesh 1 (small rectangle)
		vm[1] = {
			point3{0.0, 0.0, 0.0},
			point3{1.0, 0.0, 0.0},
			point3{1.0, 3.0, 0.0},
			point3{0.0, 3.0, 0.0}
		};
		// Area = 3.0

		// Mesh 2 (tilted parallelogram)
		vm[2] = {
			point3{0.0, 0.0, 0.0},
			point3{3.0, 0.0, 1.0},
			point3{4.0, 2.0, 1.0},
			point3{1.0, 2.0, 0.0}
		};
		// Area ≈ 6.403124237

		expectedVm[0] = vm[1];
		expectedVm[1] = vm[2];
		expectedVm[2] = vm[0];

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto ms = std::make_unique<mesh>(vm[i], vtm[i], vnm[i], std::move(mat));
			auto ms2 = std::make_unique<mesh>(expectedVm[i], vtm[i], vnm[i], std::move(mat2));
			world->objects[indx[pos++]] = std::move(ms);
			expectedWorld->add(std::move(ms2));
		}

		// quads
		point3 Q{ 0.0,0.0,0.0 };
		vec3 uq[3];
		vec3 vq[3];
		vec3 expectedUqs[3];
		vec3 expectedVqs[3];

		// Quad 1 (XZ plane)
		uq[0] = vec3{ 3.0, 0.0, 0.0 };
		vq[0] = vec3{ 0.0, 0.0, 4.0 };
		// Area = 12.0

		// Quad 2 (YZ plane)
		uq[1] = vec3{ 0.0, 2.0, 0.0 };
		vq[1] = vec3{ 0.0, 0.0, 3.0 };
		// Area = 6.0

		// Quad 3 (3D skew)
		uq[2] = vec3{ 2.0, 1.0, 1.0 };
		vq[2] = vec3{ 1.0, 3.0, 2.0 };
		// cross = (-1, -3, 5) → |.| = sqrt(35)
		// Area ≈ 5.916079783

		expectedUqs[0] = uq[2];
		expectedUqs[1] = uq[1];
		expectedUqs[2] = uq[0];
		expectedVqs[0] = vq[2];
		expectedVqs[1] = vq[1];
		expectedVqs[2] = vq[0];

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto qd = std::make_unique<quad>(Q, uq[i], vq[i], std::move(mat));
			auto qd2 = std::make_unique<quad>(Q, expectedUqs[i], expectedVqs[i], std::move(mat2));
			world->objects[indx[pos++]] = std::move(qd);
			expectedWorld->add(std::move(qd2));
		}

		// sphere
		std::vector<point3> centers, sortedCenters;
		std::vector<double> radius, sortedRadius;


		centers.reserve(3);
		centers.emplace_back(1.0, 1.0, 1.0);
		centers.emplace_back(-2.0, -2.0, -2.0);
		centers.emplace_back(4.5, -7.0, 3.3);

		radius.reserve(3);
		radius.push_back(7.7);
		radius.push_back(0.5);
		radius.push_back(12.3);

		sortedCenters.reserve(3);
		sortedCenters.emplace_back(-2.0, -2.0, -2.0);
		sortedCenters.emplace_back(1.0, 1.0, 1.0);
		sortedCenters.emplace_back(4.5, -7.0, 3.3);

		sortedRadius.reserve(3);
		sortedRadius.push_back(0.5);
		sortedRadius.push_back(7.7);
		sortedRadius.push_back(12.3);

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto sphr = std::make_unique<sphere>(centers[i], radius[i], std::move(mat));
			auto sphr2 = std::make_unique<sphere>(sortedCenters[i], sortedRadius[i], std::move(mat2));
			world->objects[indx[pos++]] = std::move(sphr);
			expectedWorld->add(std::move(sphr2));
		}

		// triangle meshes
		std::array<point3, 3> vs[3];
		std::array<point3, 3> expectedVs[3];
		std::array<point3, 3> vts[3] =
		{
			{point3{},point3{},point3{}},
			{point3{},point3{},point3{}},
			{point3{},point3{},point3{}}
		};
		std::array<point3, 3> vns[3] =
		{
			{point3{},point3{},point3{}},
			{point3{},point3{},point3{}},
			{point3{},point3{},point3{}}
		};

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

		// expected values
		expectedVs[0] = vs[0]; // 2.0
		expectedVs[1] = vs[2]; // 3.4641
		expectedVs[2] = vs[1]; // 6.0

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto trng = std::make_unique<triangle_mesh>(vs[i], vts[i], vns[i], std::move(mat));
			auto trng2 = std::make_unique<triangle_mesh>(expectedVs[i], vts[i], vns[i], std::move(mat2));
			world->objects[indx[pos++]] = std::move(trng);
			expectedWorld->add(std::move(trng2));
		}

	}

	SECTION("Case-2")
	{
		// meshes
		std::array<point3, 4> vm[3];
		std::array<point3, 4> expectedVm[3];
		std::array<point3, 4> vtm[3] =
		{
			{point3{},point3{},point3{},point3{}},
			{point3{},point3{},point3{},point3{}},
			{point3{},point3{},point3{},point3{}}
		};
		std::array<point3, 4> vnm[3] =
		{
			{point3{},point3{},point3{},point3{}},
			{point3{},point3{},point3{},point3{}},
			{point3{},point3{},point3{},point3{}}
		};

		// Mesh 0
		vm[0] = {
			point3{0.0, 0.0, 0.0},
			point3{2.0, 0.0, 0.0},
			point3{2.0, 2.0, 0.0},
			point3{0.0, 2.0, 0.0}
		};
		// Area = 4.0

		// Mesh 1
		vm[1] = {
			point3{0.0, 0.0, 0.0},
			point3{4.0, 0.0, 0.0},
			point3{5.0, 2.0, 0.0},
			point3{1.0, 2.0, 0.0}
		};
		// Area = 8.0

		// Mesh 2
		vm[2] = {
			point3{0.0, 0.0, 0.0},
			point3{2.0, 0.0, 1.0},
			point3{3.0, 3.0, 1.0},
			point3{1.0, 3.0, 0.0}
		};
		// Area ≈ 6.708203932

		expectedVm[0] = vm[0]; // 4.0
		expectedVm[1] = vm[2]; // 6.7082
		expectedVm[2] = vm[1]; // 8.0

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto ms = std::make_unique<mesh>(vm[i], vtm[i], vnm[i], std::move(mat));
			auto ms2 = std::make_unique<mesh>(expectedVm[i], vtm[i], vnm[i], std::move(mat2));
			world->objects[indx[pos++]] = std::move(ms);
			expectedWorld->add(std::move(ms2));
		}

		// quads
		point3 Q{ 0.0,0.0,0.0 };
		vec3 uq[3];
		vec3 vq[3];
		vec3 expectedUqs[3];
		vec3 expectedVqs[3];

		// Quad 0
		uq[0] = vec3{ 2.0, 0.0, 0.0 };
		vq[0] = vec3{ 0.0, 3.0, 0.0 };
		// Area = 6.0

		// Quad 1
		uq[1] = vec3{ 1.0, 1.0, 0.0 };
		vq[1] = vec3{ 2.0, -1.0, 0.0 };
		// cross = (0,0,-3) -> Area = 3.0

		// Quad 2
		uq[2] = vec3{ 2.0, 1.0, 2.0 };
		vq[2] = vec3{ 1.0, 3.0, 0.0 };
		// cross = (-6, 2, 5) -> |.| = sqrt(65)
		// Area ≈ 8.062257748

		expectedUqs[0] = uq[1]; // 3.0
		expectedUqs[1] = uq[0]; // 6.0
		expectedUqs[2] = uq[2]; // 8.0623

		expectedVqs[0] = vq[1];
		expectedVqs[1] = vq[0];
		expectedVqs[2] = vq[2];

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto qd = std::make_unique<quad>(Q, uq[i], vq[i], std::move(mat));
			auto qd2 = std::make_unique<quad>(Q, expectedUqs[i], expectedVqs[i], std::move(mat2));
			world->objects[indx[pos++]] = std::move(qd);
			expectedWorld->add(std::move(qd2));
		}

		// sphere
		std::vector<point3> centers, sortedCenters;
		std::vector<double> radius, sortedRadius;

		centers.reserve(3);
		centers.emplace_back(5.0, 0.0, -1.0);
		centers.emplace_back(-1.0, 2.0, 3.0);
		centers.emplace_back(0.5, -4.0, 2.0);

		radius.reserve(3);
		radius.push_back(9.1);
		radius.push_back(1.2);
		radius.push_back(4.4);

		sortedCenters.reserve(3);
		sortedCenters.emplace_back(-1.0, 2.0, 3.0);   // 1.2
		sortedCenters.emplace_back(0.5, -4.0, 2.0);   // 4.4
		sortedCenters.emplace_back(5.0, 0.0, -1.0);   // 9.1

		sortedRadius.reserve(3);
		sortedRadius.push_back(1.2);
		sortedRadius.push_back(4.4);
		sortedRadius.push_back(9.1);

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto sphr = std::make_unique<sphere>(centers[i], radius[i], std::move(mat));
			auto sphr2 = std::make_unique<sphere>(sortedCenters[i], sortedRadius[i], std::move(mat2));
			world->objects[indx[pos++]] = std::move(sphr);
			expectedWorld->add(std::move(sphr2));
		}

		// triangle meshes
		std::array<point3, 3> vs[3];
		std::array<point3, 3> expectedVs[3];
		std::array<point3, 3> vts[3] =
		{
			{point3{},point3{},point3{}},
			{point3{},point3{},point3{}},
			{point3{},point3{},point3{}}
		};
		std::array<point3, 3> vns[3] =
		{
			{point3{},point3{},point3{}},
			{point3{},point3{},point3{}},
			{point3{},point3{},point3{}}
		};

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
			point3{0.0, 0.0, 1.0},
			point3{2.0, 0.0, 1.0},
			point3{0.0, 2.0, 2.0}
		};
		// u = (2,0,0), v = (0,2,1), |u x v| = sqrt(20)
		// Area ≈ 2.236067977

		expectedVs[0] = vs[0]; // 2.0
		expectedVs[1] = vs[2]; // 2.2361
		expectedVs[2] = vs[1]; // 6.0

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto trng = std::make_unique<triangle_mesh>(vs[i], vts[i], vns[i], std::move(mat));
			auto trng2 = std::make_unique<triangle_mesh>(expectedVs[i], vts[i], vns[i], std::move(mat2));
			world->objects[indx[pos++]] = std::move(trng);
			expectedWorld->add(std::move(trng2));
		}
	}



	// sorting the objects in the hittable_list
	world->sort();

	// comparing the results
	REQUIRE_THAT(world.get(), hittableListMatcher(expectedWorld.get(), 1e-6));
}

TEST_CASE("Testing the resize function of the hittable_list")
{
	std::unique_ptr<hittable_list> world;
	world = std::make_unique<hittable_list>();
	int expectedSize = 0;
	SECTION("resize(5)")
	{
		world->resize(5);
		expectedSize = 5;
	}
	SECTION("resize(4)")
	{
		world->resize(4);
		expectedSize = 4;
	}
	SECTION("resize(20)")
	{
		world->resize(20);
		expectedSize = 20;
	}
	SECTION("resize(10)")
	{
		world->resize(10);
		expectedSize = 10;
	}
	int size = static_cast<int>(world->size());
	REQUIRE(size == expectedSize);
}