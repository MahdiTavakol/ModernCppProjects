#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include "../Input/input.h"
#include "../Geometry/triangle_mesh.h"
#include <sstream>

class create_input
{
public:
	create_input(std::vector<std::string> argv_vec_,
		std::vector<std::reference_wrapper<std::ostream>>& outStream_)
	{
		argc = static_cast<int>(argv_vec_.size());
		argv = new char* [argc];
		for (int iarg = 0; iarg < argc; iarg++)
		{
			int size = static_cast<int>(argv_vec_[iarg].size());
			argv[iarg] = new char[size + 1];
			std::memcpy(argv[iarg], argv_vec_[iarg].c_str(), size + 1);
		}

		std::unique_ptr<input> in = std::make_unique<input>(argc, argv, 0, outStream_);
	}

	~create_input()
	{
		for (int iarg = 0; iarg < argc; iarg++)
		{
			delete[] argv[iarg];
		}
		delete[] argv;
	}

	std::unique_ptr<input> return_input_ptr()
	{
		return std::move(input_ptr);
	}

private:
	char** argv;
	int argc;
	std::unique_ptr<input> input_ptr;

};

class vec3Matcher : public Catch::Matchers::MatcherGenericBase {
public:
	vec3Matcher(const vec3& expected_, double tol_):
		tol{ tol_ },
		expectedRef{std::ref(expected_)}
	{}

	bool match(const vec3& value_) const
	{
		return equal(expectedRef.get(),value_,tol);
	}

	std::string describe() const override
	{
		std::string message = "Comparing two vec3 with the tolerance of " + std::to_string(tol);
		return message;
	}

private:
	double tol;
	std::reference_wrapper<const vec3> expectedRef;
};

TEST_CASE("Testing reading the argv with the input class")
{
	std::vector<std::string> argv_vec;
	argv_vec.push_back("renderer");
	argv_vec.push_back("-image_width");
	argv_vec.push_back("640");
	argv_vec.push_back("-samples_per_pixel");
	argv_vec.push_back("240");
	argv_vec.push_back("-max_depth");
	argv_vec.push_back("96");
	argv_vec.push_back("-vfov");
	argv_vec.push_back("45");
	argv_vec.push_back("-aspect_ratio");
	argv_vec.push_back("16");
	argv_vec.push_back("9");
	argv_vec.push_back("-fps");
	argv_vec.push_back("120");
	argv_vec.push_back("-num_seconds");
	argv_vec.push_back("85");

	
	std::vector<std::string> expectedLog;
	expectedLog.push_back("The input: ");
	expectedLog.push_back("renderer -image_width 640 -samples_per_pixel 240 ");
	expectedLog.push_back("-max_depth 96 -vfov 45 -aspect_ratio ");
	expectedLog.push_back("16 9 -fps 120 -num_seconds ");
	expectedLog.push_back("85 ");
	expectedLog.push_back("");
	expectedLog.push_back("image_width = 640");
	expectedLog.push_back("samples_per_pixel = 240");
	expectedLog.push_back("max_depth = 96");
	expectedLog.push_back("vfov = 45");
	expectedLog.push_back("aspect_ratio = 16/9");
	expectedLog.push_back("fps = 120");
	expectedLog.push_back("num_seconds = 85");


	std::ostringstream oss;
	std::vector<std::reference_wrapper<std::ostream>> outStream =
	{
		std::ref(oss)
	};

	create_input crt_input(argv_vec, outStream);
	auto in = crt_input.return_input_ptr();


	std::string oss_string = oss.str();
	std::stringstream iss(oss_string);
	for (int i = 0; i < expectedLog.size(); i++)
	{
		std::string output;
		std::getline(iss, output);
 		REQUIRE(expectedLog[i] == output);
	}
}


TEST_CASE("Testing the compare function of the vec3")
{
	vec3 a{ 1.0, 2.0, 3.0 };
	vec3 b{ 1.0, 2.0, 3.0 };
	vec3 c{ 1.0, 2.0, 4.0 };
	vec3 d{ 1.0, 2.0, 2.0 };
	vec3 e{ 1.0, 1.0, 3.0 };
	vec3 f{ 1.0, 4.0, 3.0 };
	vec3 g{ 0.0, 2.0, 3.0 };
	vec3 h{ 2.0, 2.0, 3.0 };

	REQUIRE(a == b);
	REQUIRE(a < c);
	REQUIRE(a > d);
	REQUIRE(a > e);
	REQUIRE(a < f);
	REQUIRE(a > g);
	REQUIRE(a < h);
}

TEST_CASE("Testing the x(), y(), z() and [] functions of the vec3")
{
	vec3 a{ 2.0,3.5,4.8 };

	double x = a.x();
	double y = a.y();
	double z = a.z();

	REQUIRE_THAT(x, Catch::Matchers::WithinAbs(2.0, 1e-6));
	REQUIRE_THAT(y, Catch::Matchers::WithinAbs(3.5, 1e-6));
	REQUIRE_THAT(z, Catch::Matchers::WithinAbs(4.8, 1e-6));
	REQUIRE_THAT(a[0], Catch::Matchers::WithinAbs(2.0, 1e-6));
	REQUIRE_THAT(a[1], Catch::Matchers::WithinAbs(3.5, 1e-6));
	REQUIRE_THAT(a[2], Catch::Matchers::WithinAbs(4.8, 1e-6));
}

TEST_CASE("Testing the vec3 functions")
{
	// inputs
	vec3 a{ 1.5,8.2,6.5 };
	vec3 b = a;
	vec3 c = a;
	vec3 d = a;


	// operations
	vec3 e = -a;
	b *= 3.2;
	c += vec3{ 2.2,3.6,8.5 };
	d /= 2.0;

	// functions
	double length = a.length();
	double length_sq = a.length_squared();

	// expected results
	vec3 expected_e = { -1.5,-8.2,-6.5 };
	vec3 expected_b = { 4.8,26.24,20.8 };
	vec3 expected_c = { 3.7,11.8,15.0 };
	vec3 expected_d = { 0.75,4.1,3.25 };
	double expected_length = 10.57071426158138;
	double expected_length_sq = 111.74;


	// checking results
	REQUIRE_THAT(e, vec3Matcher(expected_e, 1e-6));
	REQUIRE_THAT(b, vec3Matcher(expected_b, 1e-6));
	REQUIRE_THAT(c, vec3Matcher(expected_c, 1e-6));
	REQUIRE_THAT(d, vec3Matcher(expected_d, 1e-6));
	REQUIRE_THAT(length, Catch::Matchers::WithinAbs(expected_length, 1e-6));
	REQUIRE_THAT(length_sq, Catch::Matchers::WithinAbs(expected_length_sq, 1e-6));
}

class fake_camera : public camera
{
public:
	fake_camera(vec3 min_, vec3 max_,
				std::array<int,2> imageSize_,
		        color background_ = color{0.0,0.0,0.0},
		        color rendercolor_ = color{1.0,1.0,1.0}) :
		min{min_}, max{max_},
		imageSize{imageSize_},
		background{background_},
		rendercolor{rendercolor_}
	{
		initialize_storage();
	}

	virtual void render(const hittable& object) override
	{
		double xPerDot = (max.x() - min.x()) / static_cast<double>(imageSize[0]);
		double yPerDot = (max.y() - min.y()) / static_cast<double>(imageSize[1]);
		for (int j = 0; j < imageSize[1]; j++)
		{
			for (int i = 0; i < imageSize[0]; i++)
			{
				double locX = i * xPerDot;
				double locY = j * yPerDot;
				vec3 ray_origin{ locX,locY,50.0 };
				vec3 ray_direction{ 0.0,0.0,-1.0 };
				double ray_time{ 0.0 };
				ray myray{ ray_origin,ray_direction,ray_time };
				color pixel_color = ray_color(myray, 0.0, object);
				color_data** c_data = c_array->return_array();
				c_data[i][j].r = pixel_color.x();
				c_data[i][j].g = pixel_color.y();
				c_data[i][j].b = pixel_color.z();
			}
		}
	}

	virtual color ray_color(const ray& r_, int depth_, const hittable& object_) const override
	{
		hit_record rec;
		if (!object_.hit(r_, interval(0.001, infinity), rec))
			return background;
		return rendercolor;
	}


protected:
	void initialize_storage() override
	{
		c_array = std::make_unique<color_array>(imageSize[0], imageSize[1]);
	}

private:
	vec3 min, max;
	std::array<int,2> imageSize;
	color background;
	color rendercolor;
};

class fake_material : public material
{
public:
	virtual ~fake_material() override = default;

	virtual bool is_equal(const material& _second) const override 
	{
		return false;
	}
};

class colorArrayMatcher : public Catch::Matchers::MatcherGenericBase {
public:
	colorArrayMatcher(
		color_array* expected_,
		double tol_,
		std::vector<std::array<int, 2>>& ignoredCoors_
		) :
		tol{ tol_ },
		expected{expected_},
		ignoredCoors{std::move(ignoredCoors_)}
	{}

	colorArrayMatcher(
		color_array* expected_,
		double tol_
	) :
		tol{ tol_ },
		expected{ expected_ }
	{
		ignoredCoors = std::vector<std::array<int, 2>>{};
	}

	bool match(color_array* value_) const
	{
		return expected->equal(value_, tol,ignoredCoors);
	}

	std::string describe() const override
	{
		std::string message = "Comparing two color_arrays with the tolerance of " + std::to_string(tol);
		return message;
	}

private:
	double tol;
	color_array* expected;
	std::vector<std::array<int, 2>> ignoredCoors;
};

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

			c_array.reset_size(num_points,num_points);

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

	SECTION("a = 7, b = 18, c = 30, d = 50")
	{
		set_params_alpha_deg(7, 18, 30, 50);
	}

	SECTION("a = 3, b = 25, c = 25, d = 25")
	{
		set_params_alpha_deg(3, 25, 25, 25);
	}

	auto camera = fake_camera{min,max,imageSize,background,rendercolor};
	camera.render(*mesh);


 	color_array* c_array_ptr_expected = &c_array;
	color_array* c_array_ptr_camera = camera.return_color_array_ptr();


	
	REQUIRE_THAT(c_array_ptr_camera, colorArrayMatcher(c_array_ptr_expected, 1e-6,ignoredCoors));

}

