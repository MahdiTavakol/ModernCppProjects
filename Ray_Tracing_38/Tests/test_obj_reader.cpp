#include "catch_amalgamated.hpp"
#include "test_shared.h"
#include "../Input/obj_model_reader.h"

std::unique_ptr<std::iostream> fake_obj_file_contents(
	std::vector<point3>& vs_,
	std::vector<point3>& vts_,
	std::vector<point3>& vns_,
	std::vector<std::vector<int>> fs_
)
{
	struct {
		std::string letter;
		std::vector<point3>& data;
		std::string nText;
	} dataVec[3] = {
		{"v",vs_, "vertices"},
		{"vn", vns_, "vertex normals"},
		{"vt", vts_, "texture coords"},
	};


	std::string contents;

	for (auto& a : dataVec)
	{
		int nVert = 0;
		for (int i = 0; i < a.data.size(); i++) {
			contents += a.letter + " ";
			for (int j = 0; j < 3; j++)
			{
				contents += std::to_string(a.data[i][j]);
				contents += " ";
			}
			contents += "\n";
			nVert++;
		}
		contents += "# ";
		contents += std::to_string(nVert) + " " + a.nText + "\n\n";
	}
	contents += "usemtl material1\n";
	for (auto& face : fs_) {
		contents +=
			"f";
		for (auto& f : face) {
			contents += " " + std::to_string(f);
		}
		contents += "\n";
	}

	auto iss_ptr = std::make_unique<std::stringstream>(contents);
	return iss_ptr;
}


std::unique_ptr<std::iostream> fake_mtl_file_contents(
	const color Kd, color Tf, color Ks,
	double Ns, double d, double Tr
)
{
	std::string contents;
	contents = "newmtl material1\n";
	contents += "Ns " + std::to_string(Ns) + "\n";
	contents += "d " + std::to_string(d) + "\n";
	contents += "Tr " + std::to_string(Tr) + "\n";
	contents += "Tf " + std::to_string(Tf[0]) + " " + std::to_string(Tf[1]) + " " + std::to_string(Tf[2]) + "\n";
	contents += "illum 2\n";
	contents += "Ka 0.6510 0.8980 0.8980\n";
	contents += "Kd " + std::to_string(Kd[0]) + " " + std::to_string(Kd[1]) + " " + std::to_string(Kd[2]) + "\n";
	contents += "Ks " + std::to_string(Ks[0]) + " " + std::to_string(Ks[1]) + " " + std::to_string(Ks[2]) + "\n";

	auto iss_ptr = std::make_unique<std::stringstream>(contents);
	return iss_ptr;
}



TEST_CASE("Testing the obj_model_reader class")
{
	// creating the input streams
	std::unique_ptr<std::iostream> obj_iss;
	std::unique_ptr<std::iostream> mtl_iss;

	// the parallel object
	char** argv;
	argv = nullptr;
	int argc = 0;
	communicator* para = new fake_parallel{};


	// material properties
	color Kd = color{ 0.6510,0.8980,0.8980 };
	double Ns = 32.0;
	double d = 1.0;
	double Tr = 0.0;
	color Tf = color{ 1.0,1.0,1.0 };
	color Ks = color{ 0.35,0.35,0.35 };

	std::vector<point3> vs, vts, vns;
	std::vector<std::vector<int>> fs;

	// expected results
	std::unique_ptr<hittable> expectedObject;
	std::unique_ptr<hittable_list> expectedWorld;
	expectedWorld = std::make_unique<hittable_list>();


	SECTION("Triangles")
	{
		fs = std::vector<std::vector<int>>{ {1,2,3} };
		SECTION("Triangle-1")
		{
			vs = std::vector<point3>{
				 point3{0.0,0.0,0.0},
				 point3{1.0,0.0,0.0},
				 point3{0.0,1.0,0.0}
			};
			vts = std::vector<point3>{
				point3{0.0,0.0,1.0},
				point3{0.0,0.0,1.0},
				point3{0.0,0.0,1.0}
			};
			vns = std::vector<point3>{
				point3{0.0,0.0,0.0},
				point3{0.0,0.0,0.0},
				point3{0.0,0.0,0.0}
			};
		}

		SECTION("Triangle-2")
		{
			vs = std::vector<point3>{
				point3{0.73, -1.21, 2.45},
				point3{-0.58, 0.91, 1.12},
				point3{1.34, 0.27, -0.76}
			};

			vts = std::vector<point3>{
				point3{0.12, 0.85, 0.33},
				point3{0.67, 0.14, 0.92},
				point3{0.45, 0.66, 0.21}
			};

			vns = std::vector<point3>{
				point3{0.0, 0.0, 1.0},
				point3{0.0, 1.0, 0.0},
				point3{1.0, 0.0, 0.0}
			};
		}

		SECTION("Triangle-3")
		{
			vs = std::vector<point3>{
				point3{-1.12, 2.34, 0.56},
				point3{0.98, -0.45, 1.73},
				point3{1.56, 1.12, -0.89}
			};

			vts = std::vector<point3>{
				point3{0.21, 0.44, 0.78},
				point3{0.83, 0.12, 0.35},
				point3{0.59, 0.91, 0.67}
			};

			vns = std::vector<point3>{
				point3{0.577, 0.577, 0.577},
				point3{-0.707, 0.707, 0.0},
				point3{0.408, -0.408, 0.816}
			};
		}

		SECTION("Triangle-4")
		{
			vs = std::vector<point3>{
				point3{-1.12,  2.34,  0.56},
				point3{ 0.98, -0.45,  1.73},
				point3{ 1.56,  1.12, -0.89},
				point3{-0.67, -1.58,  0.94},
				point3{ 0.42,  0.77, -1.31},
				point3{ 1.21, -0.93,  0.28}
			};

			vts = std::vector<point3>{
				point3{0.21, 0.44, 0.78},
				point3{0.83, 0.12, 0.35},
				point3{0.59, 0.91, 0.67},
				point3{0.14, 0.68, 0.22},
				point3{0.72, 0.37, 0.89},
				point3{0.48, 0.55, 0.11}
			};

			vns = std::vector<point3>{
				point3{ 0.577,  0.577,  0.577},
				point3{-0.707,  0.707,  0.0},
				point3{ 0.408, -0.408,  0.816},
				point3{ 0.0,    0.894,  0.447},
				point3{-0.816,  0.408,  0.408},
				point3{ 0.267, -0.535,  0.802}
			};
			fs = std::vector<std::vector<int>>{
				{1,2,3},
				{4,5,6} };
		}


		int offset = 0;
		while (offset < vs.size()) {
			std::array<point3, 3> expectedVs;
			std::array<point3, 3> expectedVts;
			std::array<point3, 3> expectedVns;
			for (int i = 0; i < 3; i++)
			{
				expectedVs[i] = vs[i + offset];
				expectedVts[i] = vts[i + offset];
				expectedVns[i] = vns[i + offset];
			}
			offset += 3;
			std::unique_ptr<material> expectedMat =
				std::make_unique<general>(Kd, Ns, d, Tr, Tf, Ks);
			expectedObject =
				std::make_unique<triangle_mesh>(
					expectedVs,
					expectedVts,
					expectedVns,
					std::move(expectedMat));
			expectedWorld->add(std::move(expectedObject));
		}
	}

	SECTION("Meshes")
	{
		fs = std::vector<std::vector<int>>{ {1,2,3,4} };
		SECTION("Mesh-1")
		{
			vs = std::vector<point3>{
				point3{-0.82,  1.47,  0.33},
				point3{ 1.25, -0.64,  0.91},
				point3{ 0.58,  1.12, -1.34},
				point3{-1.10, -0.75, -0.28}
			};

			vts = std::vector<point3>{
				point3{0.15, 0.72, 0.40},
				point3{0.88, 0.23, 0.67},
				point3{0.41, 0.95, 0.12},
				point3{0.63, 0.37, 0.81}
			};

			vns = std::vector<point3>{
				point3{ 0.0,  0.0,  1.0},
				point3{ 0.0,  1.0,  0.0},
				point3{ 1.0,  0.0,  0.0},
				point3{-0.577, 0.577, 0.577}
			};
		}
		SECTION("Mesh-2")
		{
			vs = std::vector<point3>{
				point3{ 0.91, -1.38,  0.72},
				point3{-1.44,  0.53,  1.19},
				point3{ 1.08,  1.27, -0.95},
				point3{-0.66, -0.84, -1.41}
			};

			vts = std::vector<point3>{
				point3{0.32, 0.18, 0.74},
				point3{0.79, 0.56, 0.21},
				point3{0.47, 0.93, 0.65},
				point3{0.11, 0.67, 0.38}
			};

			vns = std::vector<point3>{
				point3{ 0.707,  0.0,    0.707},
				point3{-0.408,  0.816,  0.408},
				point3{ 0.577, -0.577,  0.577},
				point3{-0.894,  0.0,    0.447}
			};
		}
		SECTION("Mesh-3")
		{
			vs = std::vector<point3>{
				point3{-1.73,  0.62,  1.08},
				point3{ 0.49, -1.26,  0.37},
				point3{ 1.64,  0.88, -1.22},
				point3{-0.95, -0.54, -0.79}
			};

			vts = std::vector<point3>{
				point3{0.26, 0.81, 0.14},
				point3{0.68, 0.09, 0.57},
				point3{0.93, 0.47, 0.32},
				point3{0.41, 0.63, 0.88}
			};

			vns = std::vector<point3>{
				point3{ 0.0,     0.707,  0.707},
				point3{ 0.816,  -0.408,  0.408},
				point3{-0.577,   0.577,  0.577},
				point3{ 0.408,   0.816, -0.408}
			};
		}

		std::array<point3, 4> expectedVs;
		std::array<point3, 4> expectedVts;
		std::array<point3, 4> expectedVns;

		for (int i = 0; i < 4; i++)
		{
			expectedVs[i] = vs[i];
			expectedVts[i] = vts[i];
			expectedVns[i] = vns[i];
		}

		std::unique_ptr<material> expectedMat =
			std::make_unique<general>(Kd, Ns, d, Tr, Tf, Ks);
		expectedObject =
			std::make_unique<mesh>(
				expectedVs,
				expectedVts,
				expectedVns,
				std::move(expectedMat));

		expectedWorld = std::make_unique<hittable_list>(std::move(expectedObject));

	}

	// creating the data streams
	obj_iss = fake_obj_file_contents(vs, vts, vns, fs);
	mtl_iss = fake_mtl_file_contents(Kd, Tf, Ks, Ns, d, Tr);

	// reading the data files
	auto reader_ptr =
		std::make_unique<obj_model_reader>(std::move(obj_iss), std::move(mtl_iss), para);
	reader_ptr->read();
	std::unique_ptr<hittable_list> world = reader_ptr->return_world();

	// comparing the results
	REQUIRE_THAT(world.get(), hittableListMatcher(expectedWorld.get(), 1e-6));

}