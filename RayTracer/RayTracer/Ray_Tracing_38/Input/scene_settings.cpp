#include "scene_settings.h"
#include <fstream>

scene_settings::scene_settings(int mode_) :
	settings{ mode_ }
{
	set_scene_map();
	set_input_map();
}

void scene_settings::set_input_map()
{
	singleInputMap = {
		{"-mode",&mode_string},
		{"-obj_file",&obj_file_name},
		{"-mtl_file",&mtl_file_name},

		{"--m",&mode_string},
		{"--obj",&obj_file_name},
		{"--mtl",&mtl_file_name}
	};
}

void scene_settings::set_scene_map()
{
	scene_map =
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
}


void scene_settings::set_obj_file_name(const std::string& obj_file_name_)
{
	obj_file_name = obj_file_name_;
}

void scene_settings::set_mtl_file_name(const std::string& mtl_file_name_)
{
	mtl_file_name = mtl_file_name_;
}

int scene_settings::return_mode() const
{
	return mode;
}


std::string scene_settings::return_obj_file_name() const
{
	return obj_file_name;
}

std::string scene_settings::return_mtl_file_name() const
{
	return obj_file_name;
}

void scene_settings::extra_parse()
{
	auto iter = scene_map.find(mode_string);

	if (iter == scene_map.end())
		throw std::invalid_argument("Unknown scene mode");
	else
		mode = iter->second;
}

void scene_settings::check_validity() const
{

	if (mode != OBJ_MODEL && mode != OBJ_MODEL_PARALLEL) {
		if (!obj_file_name.empty() || !mtl_file_name.empty())
			throw std::invalid_argument("The obj file name is given but the program mode is not obj reader");
	} else {
		std::ifstream obj_file(obj_file_name);
		std::ifstream mtl_file(mtl_file_name);

		if (!obj_file.is_open())
			throw std::invalid_argument("the obj_file could not open");
		if (!mtl_file.is_open())
			throw std::invalid_argument("the mtl_file could not open");
	}

}

void scene_settings::log_class_name(std::iostream& stream_) const
{
	stream_ << "Scene class options:" << std::endl << std::endl;
}