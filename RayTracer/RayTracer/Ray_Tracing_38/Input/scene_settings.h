#pragma once
#include "settings.h"
#include <string>


class scene_settings: public settings
{
public:
	scene_settings(int mode_);
	void set_input_map() override;
	void set_obj_file_name(const std::string& obj_file_name_);
	void set_mtl_file_name(const std::string& mtl_file_name_);
	void check_validity() const override;
	void log_class_name(std::iostream& stream_) const override;
	void extra_parse() override;


	int return_mode() const;
	std::string return_obj_file_name() const;
	std::string return_mtl_file_name() const;


protected:
	std::string mode_string;
	std::string obj_file_name = "";
	std::string mtl_file_name = "";
	//std::string obj_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_obj.obj";
	//std::string mtl_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_mtl.mtl";


	void set_scene_map();
	std::map<std::string, int> scene_map;
};
