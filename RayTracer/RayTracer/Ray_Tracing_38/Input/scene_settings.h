#pragma once
#include "settings.h"
#include <string>


class scene_settings: public settings
{
public:
	scene_settings(int mode_);
	void set_obj_file_name(const std::string& obj_file_name_);
	void set_mtl_file_name(const std::string& mtl_file_name_);

	int return_mode() const;
	std::string return_obj_file_name() const;
	std::string return_mtl_file_name() const;


protected:
	std::string obj_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_obj.obj";
	std::string mtl_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_mtl.mtl";
};
