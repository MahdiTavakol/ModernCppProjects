#include "scene_settings.h"

scene_settings::scene_settings(int mode_) :
	settings{ mode_ }
{}


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