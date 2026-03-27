#include "catch_amalgamated.hpp"
#include "test_shared.h"
#include "../Geometry/triangle_mesh.h"

TEST_CASE("Testing rendering of a triangle_mesh object")
{
	int num_points;
	std::vector<point3> vs;
	std::vector<point3> vns(3, point3(0.0, 0.0, 1.0));
	std::vector<point3> vts = vs;

	std::unique_ptr<material> mat =
		std::make_unique<fake_material>();
	std::unique_ptr<hittable> mesh;
	vec3 min{ 0.0,0.0,0.0 };
	vec3 max;

	std::vector<std::array<int, 2>> ignoredCoors{};

	std::array<int, 2> imageSize;


	color background{ 0.0,0.0,0.0 };
	color rendercolor{ 1.0,1.0,1.0 };

	// the expected array
	color_array c_array;

	auto set_params_45deg = [&](const int& num_points)
		{
			double num_points_double = static_cast<double>(num_points);
			vs =
			{
				{num_points_double,0.0,0.0},
				{0.0,0.0,0.0},
				{0.0,num_points_double,0.0}
			};
			vts = vs;
			max = vec3{ num_points_double,num_points_double,num_points_double };
			imageSize = { num_points,num_points };
			mesh =
				std::make_unique<triangle_mesh>(vs, vts, vns, std::move(mat));

			c_array.reset_size(num_points, num_points);

			for (int i = 0; i < num_points; i++)
				for (int j = 0; j < num_points; j++)
				{
					color c = background;
					color_data c_data;
					if (i > 0 && j > 0 && i + j < num_points)
						c = rendercolor;
					c_data.r = c[0];
					c_data.g = c[1];
					c_data.b = c[2];
					c_array(i, j) = c_data;
				}
		};

	auto set_params_alpha_deg = [&](const int& a_, const int& b_, const int& c_, const int& d_)
		{
			double a_double = static_cast<double>(a_);
			double b_double = static_cast<double>(b_);
			double c_double = static_cast<double>(c_);
			double d_double = static_cast<double>(d_);

			vs =
			{
				{a_double,0.0,0.0},
				{0.0,0.0,0.0},
				{0.0,b_double,0.0}
			};
			vts = vs;
			max = vec3{ c_double,d_double,10.0 };
			imageSize = { c_,d_ };
			mesh =
				std::make_unique<triangle_mesh>(vs, vts, vns, std::move(mat));

			c_array.reset_size(c_, d_);
			for (int i = 0; i < c_; i++)
				for (int j = 0; j < d_; j++)
				{
					color clr = background;
					color_data c_data;
					if (i > 0 && j > 0 && b_ * i + a_ * j < a_ * b_)
						clr = rendercolor;
					c_data.r = clr[0];
					c_data.g = clr[1];
					c_data.b = clr[2];
					c_array(i, j) = c_data;
				}
		};

	auto set_params_3D = [&](const int& a_, const int& b_,
		const int& c_, const int& d_,
		const int& e_, const int& f_)
		{
			double a_double = static_cast<double>(a_);
			double b_double = static_cast<double>(b_);
			double c_double = static_cast<double>(c_);
			double d_double = static_cast<double>(d_);
			double e_double = static_cast<double>(e_);
			double f_double = static_cast<double>(f_);

			vs =
			{
				{a_double,0.0,e_double},
				{0.0,0.0,f_double},
				{0.0,b_double,0.0}
			};
			vts = vs;
			max = vec3{ c_double,d_double,10.0 };
			imageSize = { c_,d_ };
			mesh =
				std::make_unique<triangle_mesh>(vs, vts, vns, std::move(mat));

			c_array.reset_size(c_, d_);
			for (int i = 0; i < c_; i++)
				for (int j = 0; j < d_; j++)
				{
					color clr = background;
					color_data c_data;
					if (i == 0 || j == 0 || b_ * i + a_ * j == a_ * b_)
						ignoredCoors.push_back(std::array<int, 2>{i, j});
					if (i > 0 && j > 0 && b_ * i + a_ * j < a_ * b_)
						clr = rendercolor;
					c_data.r = clr[0];
					c_data.g = clr[1];
					c_data.b = clr[2];
					c_array(i, j) = c_data;
				}
		};

	SECTION("45 deg num_points = 10")
	{
		set_params_45deg(10);
	}

	SECTION("45 deg num_points = 20")
	{
		set_params_45deg(20);
	}

	SECTION("45 deg num_points = 30")
	{
		set_params_45deg(30);
	}

	SECTION("45 deg num_points = 40")
	{
		set_params_45deg(40);
	}

	SECTION("45 deg num_points = 50")
	{
		set_params_45deg(50);
	}

	SECTION("a = 10, b = 5, c = 15, d = 20")
	{
		set_params_alpha_deg(10, 5, 15, 20);
	}

	SECTION("a = 7, b = 18, c = 30, d = 50")
	{
		set_params_alpha_deg(7, 18, 30, 50);
	}

	SECTION("a = 3, b = 25, c = 25, d = 25")
	{
		set_params_alpha_deg(3, 25, 25, 25);
	}

	SECTION("a = 10, b = 5, c = 15, d = 20, e = 3, f = 1")
	{
		set_params_3D(10, 5, 15, 20, 3, 1);
	}

	SECTION("a = 7, b = 18, c = 30, d = 50, e = 5, f = 10")
	{
		set_params_3D(7, 18, 30, 50, 5, 10);
	}

	SECTION("a = 3, b = 25, c = 25, d = 25, e = 2, f= 5")
	{
		set_params_3D(3, 25, 25, 25, 2, 5);
	}

	auto camera = fake_camera{ min,max,imageSize,background,rendercolor };
	camera.render(*mesh);


	color_array* c_array_ptr_expected = &c_array;
	color_array* c_array_ptr_camera = camera.return_color_array_ptr();



	REQUIRE_THAT(c_array_ptr_camera, colorArrayMatcher(c_array_ptr_expected, 1e-6, ignoredCoors));

}