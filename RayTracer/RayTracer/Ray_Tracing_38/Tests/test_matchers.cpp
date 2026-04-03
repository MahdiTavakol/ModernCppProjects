#include "test_shared.h"
#include "catch_amalgamated.hpp"

#include "../Geometry/sphere.h"
#include "../Geometry/triangle_mesh.h"
#include "../Geometry/quad.h"

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

TEST_CASE("Testing hittableListMatcher")
{
	std::unique_ptr<hittable_list> world1;
	std::unique_ptr<hittable_list> world2;

	world1 = std::make_unique<hittable_list>();
	world2 = std::make_unique<hittable_list>();

	SECTION("A Simple Triangle test")
	{
		std::unique_ptr<material> mat1, mat2;

		std::array<point3, 3> vs = {
			point3{1.0,0.0,0.0},
			point3{0.0,1.0,0.0},
			point3{0.0,0.0,1.0}
		};
		std::array<point3, 3> vts = {
			point3{0.0,0.0,1.0},
			point3{1.0,0.0,0.0},
			point3{0.0,1.0,0.0}
		};
		std::array<point3, 3> vns = {
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


		world1->add(std::move(triangle1));
		world2->add(std::move(triangle2));
	}
	/*
	SECTION("Comparing a sorted with an unsorted hittable_list")
	{
		// sphere
		std::vector<point3> centers;
		std::vector<double> radius;

		centers.reserve(3);
		centers.emplace_back(5.0, 0.0, -1.0);
		centers.emplace_back(-1.0, 2.0, 3.0);
		centers.emplace_back(0.5, -4.0, 2.0);

		radius.reserve(3);
		radius.push_back(9.1);
		radius.push_back(1.2);
		radius.push_back(4.4);

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto sphr = std::make_unique<sphere>(centers[i], radius[i], std::move(mat));
			auto sphr2 = std::make_unique<sphere>(centers[i], radius[i], std::move(mat2));
			world1->add(std::move(sphr));
			world2->add(std::move(sphr2));
		}

		// triangle meshes
		std::array<point3, 3> vs[3];
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

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto trng = std::make_unique<triangle_mesh>(vs[i], vts[i], vns[i], std::move(mat));
			auto trng2 = std::make_unique<triangle_mesh>(vs[i], vts[i], vns[i], std::move(mat2));
			world1->add(std::move(trng));
			world2->add(std::move(trng2));
		}


		// quads
		point3 Q{ 0.0,0.0,0.0 };
		vec3 uq[3];
		vec3 vq[3];

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

		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<material> mat = std::make_unique<fake_material>();
			std::unique_ptr<material> mat2 = std::make_unique<fake_material>();
			auto qd = std::make_unique<quad>(Q, uq[i], vq[i], std::move(mat));
			auto qd2 = std::make_unique<quad>(Q, uq[i], vq[i], std::move(mat2));
			world1->add(std::move(qd));
			world2->add(std::move(qd2));
		}

		world1->sort();
	}
	*/

	REQUIRE_THAT(world1.get(), hittableListMatcher(world2.get(), 1e-6));
}

TEST_CASE("Point3 matcher test")
{
	point3 p1;
	point3 p2;

	SECTION("Default constructor")
	{
		p1 = point3();
		p2 = point3();
	}
	SECTION("Case 1")
	{
		p1 = point3{ 0.0, 0.0, 0.0 };
		p2 = point3{ 0.0, 0.0, 0.0 };
	}
	SECTION("Case 2")
	{
		p1 = point3{ 1.0, 2.0, 3.0 };
		p2 = point3{ 1.0, 2.0, 3.0 };
	}
	SECTION("Case 3")
	{
		p1 = point3{ -1.0, -2.0, -3.0 };
		p2 = point3{ -1.0, -2.0, -3.0 };
	}
	SECTION("Case 4")
	{
		p1 = point3{ 1e-6, 2e-6, 3e-6 };
		p2 = point3{ 1e-6, 2e-6, 3e-6 };
	}

	REQUIRE_THAT(p1, point3Matcher(p2, 1e-6));
}

TEST_CASE("Testing ostringstream matcher") {
	std::ostringstream oss;
	std::vector<std::string> expectedOutput;

	SECTION("test-1")
	{
		oss << "test1" << std::endl << "test2" << std::endl
			<< "test3 test4" << std::endl
			<< "test5 test6 test7" << std::endl;

		expectedOutput.push_back("test1");
		expectedOutput.push_back("test2");
		expectedOutput.push_back("test3 test4");
		expectedOutput.push_back("test5 test6 test7");
	}
	SECTION("test-2")
	{
		oss << "alpha42" << std::endl
			<< "beta gamma" << std::endl
			<< "delta epsilon zeta" << std::endl;

		expectedOutput.push_back("alpha42");
		expectedOutput.push_back("beta gamma");
		expectedOutput.push_back("delta epsilon zeta");
	}
	SECTION("test-3")
	{
		oss << "node_1 node_2 node_3 node_4" << std::endl
			<< "x9" << std::endl
			<< "compute value=128" << std::endl
			<< "final_state ready" << std::endl;

		expectedOutput.push_back("node_1 node_2 node_3 node_4");
		expectedOutput.push_back("x9");
		expectedOutput.push_back("compute value=128");
		expectedOutput.push_back("final_state ready");
	}
	SECTION("test-4")
	{
		oss << "init sequence start" << std::endl
			<< "param_a 77 param_b 13" << std::endl
			<< "load_data chunk1 chunk2 chunk3 chunk4" << std::endl
			<< "done" << std::endl;

		expectedOutput.push_back("init sequence start");
		expectedOutput.push_back("param_a 77 param_b 13");
		expectedOutput.push_back("load_data chunk1 chunk2 chunk3 chunk4");
		expectedOutput.push_back("done");
	}
	SECTION("test-5")
	{
		oss << "start_sequence id_001 mode_active" << std::endl
			<< "load_config path=/data/config.yaml retries=3 timeout=120" << std::endl
			<< "init_modules core io net gpu cpu memory" << std::endl
			<< "set_parameters alpha=0.25 beta=1.75 gamma=42 delta=9" << std::endl
			<< "processing batch_01 batch_02 batch_03 batch_04 batch_05" << std::endl
			<< "checkpoint saved_step_1000 saved_step_2000 saved_step_3000" << std::endl
			<< "status running iteration=57 progress=0.83" << std::endl
			<< "finalize sequence_complete success=true" << std::endl;

		expectedOutput.push_back("start_sequence id_001 mode_active");
		expectedOutput.push_back("load_config path=/data/config.yaml retries=3 timeout=120");
		expectedOutput.push_back("init_modules core io net gpu cpu memory");
		expectedOutput.push_back("set_parameters alpha=0.25 beta=1.75 gamma=42 delta=9");
		expectedOutput.push_back("processing batch_01 batch_02 batch_03 batch_04 batch_05");
		expectedOutput.push_back("checkpoint saved_step_1000 saved_step_2000 saved_step_3000");
		expectedOutput.push_back("status running iteration=57 progress=0.83");
		expectedOutput.push_back("finalize sequence_complete success=true");
	}
	SECTION("test-6")
	{
		oss << "user_login id=8421 role_admin session_start" << std::endl
			<< "query select_all from_table where_status=active order_by=date" << std::endl
			<< "response rows=128 cols=7 latency_ms=23" << std::endl
			<< "cache_update key=user_8421 value_updated timestamp=1678901234" << std::endl
			<< "stream_data packet1 packet2 packet3 packet4 packet5 packet6" << std::endl
			<< "warning threshold_exceeded metric=temperature value=98.6" << std::endl
			<< "recovery action_restart service=db_service attempts=2" << std::endl
			<< "log_entry code=200 message=operation_successful" << std::endl
			<< "session_end duration=3600 cleanup_done" << std::endl;

		expectedOutput.push_back("user_login id=8421 role_admin session_start");
		expectedOutput.push_back("query select_all from_table where_status=active order_by=date");
		expectedOutput.push_back("response rows=128 cols=7 latency_ms=23");
		expectedOutput.push_back("cache_update key=user_8421 value_updated timestamp=1678901234");
		expectedOutput.push_back("stream_data packet1 packet2 packet3 packet4 packet5 packet6");
		expectedOutput.push_back("warning threshold_exceeded metric=temperature value=98.6");
		expectedOutput.push_back("recovery action_restart service=db_service attempts=2");
		expectedOutput.push_back("log_entry code=200 message=operation_successful");
		expectedOutput.push_back("session_end duration=3600 cleanup_done");
	}



	REQUIRE_THAT(&oss, OStringStreamMatcher(&expectedOutput));
}