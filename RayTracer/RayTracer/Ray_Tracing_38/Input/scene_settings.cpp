#include "scene_settings.h"
#include <fstream>

scene_settings::scene_settings(int mode_, bool wrong_access_) :
	settings{ mode_ },
	wrong_access{wrong_access_}
{
	set_scene_map();
	set_input_map();
	set_special_map();
}

void scene_settings::set_input_map()
{
	singleInputMap = {
		{"-mode",&mode_string},
		{"-obj_file",&obj_file_name},
		{"-mtl_file",&mtl_file_name},

		// floor
		{"-floor",&floor_string},
		{"-floor_size",&floor_size},

		// light
		{"-diffuse_light",&diffuse_light_string},
		{"-light_size",&light_size},

		// scale
		{"-scale",&scale_string},
		{"-scale_factor",&scale_factor},

		// fog
		{"-fog",&fog_string},
		{"-fog_density",&fog_density},

		{"--m",&mode_string},
		{"--obj",&obj_file_name},
		{"--mtl",&mtl_file_name}
	};

	threeInputMap = {
		// floor
		{"-floor_color",std::tuple(&floor_color[0],&floor_color[1],&floor_color[2])},
		// diffuse light
		{"-light_color",std::tuple(&light_color[0],&light_color[1],&light_color[2])},
		// fog color
		{ "-fog_color",std::tuple(&fog_color[0],&fog_color[1],&fog_color[2]) }
	};
}

void scene_settings::set_scene_map()
{
	this->return_scene_map(scene_map);
}

void scene_settings::return_scene_map(std::map<std::string, int>& scene_map_)
{
	scene_map_ =
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
	return mtl_file_name;
}

void scene_settings::extra_parse()
{
	std::transform(mode_string.begin(), mode_string.end(), mode_string.begin(), ::toupper);
	auto iter = scene_map.find(mode_string);

	if (iter == scene_map.end())
		throw std::invalid_argument("Unknown scene mode");
	else
		mode = iter->second;


	for (auto& [str, num] : specialStrToInt)
	{
		std::transform(str->begin(), str->end(), str->begin(), ::toupper);
		if (!str->compare("ON") || !str->compare("TRUE"))
			specials.insert(num);
	}
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

bool scene_settings::specialCheck(specialEnum effect_) const
{
	if (specials.find(effect_) == specials.end())
		return false;
	return true;
}

void scene_settings::log_class_name(std::iostream& stream_) const
{
	stream_ << "Scene class options:" << std::endl << std::endl;
}

void scene_settings::set_special_map()
{
	specialStrToInt =
	{
		{&floor_string,specialEnum::FLOOR},
		{&diffuse_light_string,specialEnum::DIFFUSE_LIGHT},
		{&scale_string,specialEnum::SCALE},
		{&fog_string,specialEnum::FOG}
	};
}

// floor 
void scene_settings::floor_settings(color& floor_color_, double& floor_size_) const {
	floor_color_ = floor_color;
	floor_size_ = floor_size;
}

// diffuse light
void scene_settings::light_settings(color& light_color_, double& light_size_) const {
	light_color_ = light_color;
	light_size_ = light_size;
}
// scaling
void scene_settings::scale_settings(double& factor_) const
{
	factor_ = scale_factor;
}

void scene_settings::fog_settings(double& density_, color fog_color_) const
{
	density_ = fog_density;
	fog_color_ = fog_color;
}
