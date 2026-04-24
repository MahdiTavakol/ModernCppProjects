#include "scene_settings.h"
#include <fstream>

scene_settings::scene_settings(int mode_) :
	settings{ mode_ }
{
	set_input_map();
}

void scene_settings::set_input_map()
{
	singleInputMap = {
		{"-mode",&mode},
		{"-obj_file",&obj_file_name},
		{"-mtl_file",&mtl_file_name},

		{"--m",&mode},
		{"--obj",&obj_file_name},
		{"--mtl",&mtl_file_name}
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

void scene_settings::check_validity() const
{
	settings::check_validity();

	if (obj_file_name.empty())
		return;
	if (mtl_file_name.empty())
		return;

	std::ifstream obj_file(obj_file_name);
	std::ifstream mtl_file(mtl_file_name);

	if (!obj_file.is_open())
		throw std::invalid_argument("the obj_file could not open");
	if (!mtl_file.is_open())
		throw std::invalid_argument("the mtl_file could not open");

}