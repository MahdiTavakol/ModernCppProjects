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
		parallel{0,nullptr}
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
};

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

std::unique_ptr<std::istream> fake_mtl_file_contents()
{
	std::string contents;
	contents  = "newmtl material1\n";
	contents += "Ns 32\n";
	contents += "d 1\n";
	contents += "Tr 0\n";
	contents += "Tf 1 1 1\n";
	contents += "illum 2\n";
	contents += "Ka 0.6510 0.8980 0.8980\n";
	contents += "Kd 0.6510 0.8980 0.8980\n";
	contents += "Ks 0.3500 0.3500 0.3500\n";

	auto iss_ptr = std::make_unique<std::istringstream>(contents);
	return iss_ptr;
}


TEST_CASE("Testing the obj_model_reader class")
{
	auto obj_iss = fake_obj_file_contents();
	auto mtl_iss = fake_mtl_file_contents();


	char** argv;
	argv = nullptr;
	int argc = 0;
	fake_parallel para_obj{};
	parallel* para = &para_obj;
	auto reader_ptr = 
		std::make_unique<obj_model_reader>(std::move(obj_iss), std::move(mtl_iss), para);

	reader_ptr->read();
	std::unique_ptr<hittable> world = reader_ptr->return_world();
}



