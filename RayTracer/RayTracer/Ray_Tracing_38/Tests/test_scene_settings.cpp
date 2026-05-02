#include "test_shared.h"
#include "catch_amalgamated.hpp"
#include "../Input/scene_settings.h"


TEST_CASE("Testing the scene_settings")
{
	int mode = -1;
	std::unique_ptr<scene_settings> wld_settings;

	int expectedMode = 0;
	std::string in;

	std::vector<std::pair<std::string, int>> tests =
	{
		{"RANDOM_SPHERES",RANDOM_SPHERES},
		{"CHECKER_BOARDS",CHECKER_BOARDS},
		{"EARTH_SPHERE",EARTH_SPHERE},
		{"PERLIN_SPHERE",PERLIN_SPHERE},
		{"QUADS",QUADS},
		{"SIMPLE_LIGHT",SIMPLE_LIGHT},
		{"TWO_LIGHTS",TWO_LIGHTS},
		{"CORNELL_BOX",CORNELL_BOX},
		{"TWO_BOXES",TWO_BOXES},
		{"TWO_BOXES_ROTATED",TWO_BOXES_ROTATED},
		{"CORNELL_SMOKE",CORNELL_SMOKE},
		{"OBJ_MODEL",OBJ_MODEL},
		{"OBJ_MODEL_PARALLEL",OBJ_MODEL_PARALLEL},
		{"RANDOM_SPHERES_ANIMATED",RANDOM_SPHERES_ANIMATED},
		{"SIMPLE_2D_PARALEL_TEST",SIMPLE_2D_PARALEL_TEST}
	};

	for (const auto& [str, num] : tests)
	{
		SECTION(str)
		{
			in = "scene -mode " + str;
			expectedMode = num;
		}
	}


	wld_settings = std::make_unique<scene_settings>(mode, true);
	wld_settings->add_cmd(in);
	wld_settings->parse_commands();
	wld_settings->extra_parse();

	int md = wld_settings->return_mode();

	REQUIRE(md == expectedMode);
}

TEST_CASE("Testing the obj reader keywords of the scene_settings")
{
	int mode = -1;
	std::unique_ptr<scene_settings> wld_settings;

	int expectedMode = 0;
	std::string in;

	std::string obj_file, mtl_file;
	std::string expected_obj_file = "", expected_mtl_file = "";

	std::vector<std::pair<std::string, int>> tests =
	{
		{"OBJ_MODEL",OBJ_MODEL},
		{"OBJ_MODEL_PARALLEL",OBJ_MODEL_PARALLEL},
	};

	for (const auto& [str, num] : tests)
	{
		SECTION(str + "-1")
		{
			in = "scene -mode " + str + " -obj_file obj1.obj -mtl_file mtl1.mtl";
			expectedMode = num;
			expected_obj_file = "obj1.obj";
			expected_mtl_file = "mtl1.obj";
		}
		SECTION(str + "-2")
		{
			in = "scene -mode " + str + " -mtl_file mtl2.mtl -obj_file obj2.obj";
			expectedMode = num;
			expected_obj_file = "obj2.obj";
			expected_mtl_file = "mtl2.mtl";
		}
		SECTION(str + "-3")
		{
			in = "scene -mode " + str + " -obj_file obj3.obj";
			expectedMode = num;
			expected_obj_file = "obj3.obj";
		}
		SECTION(str + "-4")
		{
			in = "scene -mode " + str + " -mtl_file mtl4.mtl";
			expectedMode = num;
			expected_mtl_file = "mtl4.mtl";
		}
		SECTION(str + "-5")
		{
			in = "scene   -mode  " + str + "   -obj_file   obj5.obj   -mtl_file   mtl5.mtl";
			expectedMode = num;
			expected_obj_file = "obj5.obj";
			expected_mtl_file = "mtl5.mtl";
		}
	}


	wld_settings = std::make_unique<scene_settings>(mode, true);
	wld_settings->add_cmd(in);
	wld_settings->parse_commands();
	wld_settings->extra_parse();
	obj_file = wld_settings->return_obj_file_name();
	mtl_file = wld_settings->return_mtl_file_name();

	int md = wld_settings->return_mode();

	REQUIRE(md == expectedMode);
	REQUIRE(obj_file == expected_obj_file);
	REQUIRE(mtl_file == expected_mtl_file);
}


