#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "../Algorithms/parallel.h"
#include "../Input/obj_model_reader.h"


#include "test_shared.h"
#include <sstream>

class fake_parallel : public parallel
{
public:
	fake_parallel():
		parallel{}
	{}
	int return_rank() const override {
		return 0;
	}
	int return_size() const override {
		return 1;
	}
	std::array<int, 2> return_rank_config() const override {
		return std::array<int, 2>{0, 0};
	}
	std::array<int, 2> return_size_config() const override {
		return std::array<int, 2>{1, 1};
	}

	void gather(
		color_data* one_, 
		color_data* one_all,
		const int& num_data
	) const override
	{
		return;
	}
	void barrier() const override {
		return;
	}
};

std::unique_ptr<std::istream> fake_obj_file_contents(
	std::vector<point3> vs_,
	std::vector<point3> vts_,
	std::vector<point3> vns_,
	std::vector<std::array<int,3>> fs_
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
		contents += std::to_string(nVert) + " " + a.nText +"\n\n";
	}
	contents += "usemtl material1\n";
	for (auto& face : fs_) {
		contents += 
			"f " + 
			std::to_string(face[0]) + " " + 
			std::to_string(face[1]) + " " + 
			std::to_string(face[2]) + "\n";
	}

	auto iss_ptr = std::make_unique<std::istringstream>(contents);
	return iss_ptr;
}

std::unique_ptr<std::istream> fake_obj_file_contents()
{
	std::string contents;
	contents =  "v 0.0 0.0 0.0\n";
	contents += "v 1.0 0.0 0.0\n";
	contents += "v 0.0 1.0 0.0\n";
	contents += "# 3 vertices\n\n";
	contents += "vn 0.0 0.0 1.0\n";
	contents += "vn 0.0 0.0 1.0\n";
	contents += "vn 0.0 0.0 1.0\n";
	contents += "# 3 vertex normals\n\n";
	contents += "vt 0.0 0.0 0.0\n";
	contents += "vt 0.0 0.0 0.0\n";
	contents += "vt 0.0 0.0 0.0\n";
	contents += "# 3 texture coords\n\n";
	contents += "usemtl material1\n";
	contents += "f 1 2 3";

	auto iss_ptr = std::make_unique<std::istringstream>(contents);
	return iss_ptr;
}

std::unique_ptr<std::istream> fake_mtl_file_contents(
	const color Kd,color Tf, color Ks,
	double Ns, double d, double Tr
)
{
	std::string contents;
	contents  = "newmtl material1\n";
	contents += "Ns " + std::to_string(Ns) + "\n";
	contents += "d " + std::to_string(d) + "\n";
	contents += "Tr "+ std::to_string(Tr) + "\n";
	contents += "Tf " + std::to_string(Tf[0]) + " " + std::to_string(Tf[1]) + " " + std::to_string(Tf[2]) + "\n";
	contents += "illum 2\n";
	contents += "Ka 0.6510 0.8980 0.8980\n";
	contents += "Kd " + std::to_string(Kd[0]) + " " + std::to_string(Kd[1]) + " " + std::to_string(Kd[2]) +"\n";
	contents += "Ks " + std::to_string(Ks[0]) + " " + std::to_string(Ks[1]) + " " + std::to_string(Ks[2]) + "\n";

	auto iss_ptr = std::make_unique<std::istringstream>(contents);
	return iss_ptr;
}



TEST_CASE("Testing the obj_model_reader class")
{
	// creating the input streams
	std::unique_ptr<std::istream> obj_iss;
	std::unique_ptr<std::istream> mtl_iss;

	// the parallel object
	char** argv;
	argv = nullptr;
	int argc = 0;
	parallel* para = new fake_parallel{};


	// material properties
	color Kd = color{ 0.6510,0.8980,0.8980 };
	double Ns = 32.0;
	double d = 1.0;
	double Tr = 0.0;
	color Tf = color{ 1.0,1.0,1.0 };
	color Ks = color{ 0.35,0.35,0.35 };


	SECTION("Triangles")
	{
		std::vector<point3> vs, vts, vns;
		std::vector<std::array<int, 3>> fs = {
			{1,2,3}
		};
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
			obj_iss = fake_obj_file_contents(vs, vts, vns,fs);
			mtl_iss = fake_mtl_file_contents(Kd, Tf, Ks, Ns, d, Tr);

			std::string obj_string;
		}
		/*
		SECTION("Triangle-2")
		{

		}
		SECTION("Triangle-3")
		{

		}
		*/

		// reading the data files
		auto reader_ptr =
			std::make_unique<obj_model_reader>(std::move(obj_iss), std::move(mtl_iss), para);
		reader_ptr->read();
		std::unique_ptr<hittable_list> world = reader_ptr->return_world();



		// expected results
		std::unique_ptr<hittable_list> expectedWorld;
		std::unique_ptr<hittable> expectedTriangleMesh;

		std::vector<point3> expectedVs = vs;
		std::vector<point3> expectedVts = vts;
		std::vector<point3> expectedVns = vns;
		std::unique_ptr<material> expectedMat =
			std::make_unique<general>(Kd, Ns, d, Tr, Tf, Ks);
		expectedTriangleMesh =
			std::make_unique<triangle_mesh>(
				expectedVs,
				expectedVts,
				expectedVns,
				std::move(expectedMat));

		expectedWorld = std::make_unique<hittable_list>(std::move(expectedTriangleMesh));

		// comparing the results
		REQUIRE_THAT(world.get(), hittableListMatcher(expectedWorld.get(), 1e-6));
	}

}



