#include "test_shared.h"
#include "catch_amalgamated.hpp"

TEST_CASE("Testing colorArrayMatcher")
{
	color_array c_array_1;
	color_array c_array_2;
	c_array_1.reset_size(3, 3);
	c_array_2.reset_size(3, 3);


	c_array_1(0, 0) = color_data{ 1.0, 2.0, 3.0 };
	c_array_2(0, 0) = color_data{ 1.0, 2.0, 3.0 };

	c_array_1(0, 1) = color_data{ 0.42, 0.73, 0.15 };
	c_array_2(0, 1) = color_data{ 0.42, 0.73, 0.15 };

	c_array_1(0, 2) = color_data{ 0.91, 0.12, 0.58 };
	c_array_2(0, 2) = color_data{ 0.91, 0.12, 0.58 };

	c_array_1(1, 0) = color_data{ 0.33, 0.67, 0.25 };
	c_array_2(1, 0) = color_data{ 0.33, 0.67, 0.25 };

	c_array_1(1, 1) = color_data{ 0.88, 0.44, 0.19 };
	c_array_2(1, 1) = color_data{ 0.88, 0.44, 0.19 };

	c_array_1(1, 2) = color_data{ 0.56, 0.81, 0.36 };
	c_array_2(1, 2) = color_data{ 0.56, 0.81, 0.36 };

	c_array_1(2, 0) = color_data{ 0.14, 0.95, 0.62 };
	c_array_2(2, 0) = color_data{ 0.14, 0.95, 0.62 };

	c_array_1(2, 1) = color_data{ 0.77, 0.21, 0.49 };
	c_array_2(2, 1) = color_data{ 0.77, 0.21, 0.49 };

	c_array_1(2, 2) = color_data{ 0.65, 0.39, 0.84 };
	c_array_2(2, 2) = color_data{ 0.65, 0.39, 0.84 };

	color_array* c_array_1_ptr = &c_array_1;
	color_array* c_array_2_ptr = &c_array_2;

	REQUIRE_THAT(c_array_1_ptr, colorArrayMatcher(c_array_2_ptr, 1e-6));

}

TEST_CASE("Testing worldMatcher")
{
	std::unique_ptr<hittable_list> world1;
	std::unique_ptr<hittable_list> world2;
	std::unique_ptr<material> mat1, mat2;

	std::array<point3,3> vs = {
		point3{1.0,0.0,0.0},
		point3{0.0,1.0,0.0},
		point3{0.0,0.0,1.0}
	};
	std::array<point3,3> vts = {
		point3{0.0,0.0,1.0},
		point3{1.0,0.0,0.0},
		point3{0.0,1.0,0.0}
	};
	std::array<point3,3> vns = {
		point3{0.0,0.0,1.0},
		point3{2.0,0.0,1.0},
		point3{1.0,3.0,2.0}
	};

	mat1 = std::make_unique<fake_material>();
	mat2 = std::make_unique<fake_material>();

	std::unique_ptr<hittable> triangle1 =
		std::make_unique<triangle_mesh>(vs, vts, vns, std::move(mat1));

	std::unique_ptr<hittable> triangle2 =
		std::make_unique<triangle_mesh>(vs, vts, vns, std::move(mat2));

	//REQUIRE_THAT(world1.get(), worldMatcher(world2.get(), 1e-6));
}