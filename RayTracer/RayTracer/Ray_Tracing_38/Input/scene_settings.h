#pragma once
#include "settings.h"
#include <string>

// specials enum
enum class specialEnum {
	FLOOR,
	DIFFUSE_LIGHT,
	SCALE,
	FOG
};

class scene_settings: public settings
{
public:
	scene_settings(int mode_, bool wrong_access = false);
	void set_input_map() override;
	void set_obj_file_name(const std::string& obj_file_name_);
	void set_mtl_file_name(const std::string& mtl_file_name_);
	void check_validity() const override;
	void log_class_name(std::iostream& stream_) const override;
	void extra_parse() override;


	int return_mode() const;
	static void return_scene_map(std::map<std::string, int>& scene_map_);
	std::string return_obj_file_name() const;
	std::string return_mtl_file_name() const;
	std::string return_gltf_file_name() const;

	// special parser
	void parse_material_change(std::istringstream& iss_);


	// related to special effects
	// floor
	bool specialCheck(specialEnum effect_) const;
	void floor_settings(color& floor_color_, double& floor_size_) const;
	// diffuse light
	void light_settings(color& light_color_, double& light_size_) const;
	// scaling
	void scale_settings(double& factor_) const;
	// fog
	void fog_settings(double& density_, color fog_color_) const;




protected:
	bool wrong_access = false;
	std::string mode_string;
	std::string obj_file_name = "";
	std::string mtl_file_name = "";
	std::string gltf_file_name = "";
	//std::string obj_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_obj.obj";
	//std::string mtl_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_mtl.mtl";


	void set_scene_map();
	std::map<std::string, int> scene_map;

	// special effects
	// index
	void set_special_map();
	std::unordered_set<specialEnum> specials;
	std::map<std::string*, specialEnum> specialStrToInt;
	// each special settings
	std::string floor_string;
	color floor_color = color();
	double floor_size = 0.0;
	// diffuse light
	std::string diffuse_light_string;
	color light_color;
	double light_size;
	// scale
	std::string scale_string;
	double scale_factor;
	// fog
	std::string fog_string;
	double fog_density;
	color fog_color;

};
