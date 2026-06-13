#pragma once

#include "../Shared/rtweekend.h"


#include "../Algorithms/hittable_list.h"
#include "../Algorithms/communicator.h"
#include "../Materials/material_list.h"
#include "../Output/Logger.h"
#include "scene_settings.h"

class scene_factory {
public:
	scene_factory(settings* wld_settings, Logger* error_, communicator* para_);
	void create();
	std::unique_ptr<hittable_list> return_object();
	std::unique_ptr<material_list> return_mtl_list();

protected:
	const int mode;
	std::unique_ptr<hittable_list> world;
	std::unique_ptr<material_list> list;
	communicator* para;
	Logger* error;
	BVH_Split_Method bvh_mode = BVH_Split_Method::MEDIAN;


	// filenames for 3d object loading.. In the future 
	// I will make the file names more general
	std::string obj_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_obj.obj";
	std::string mtl_file_name;
	std::string gltf_file_name;

	// settings
	scene_settings* stngs;

private:
	void setup_random_spheres();
	void setup_checker_boards();
	void setup_earth_sphere();
	void setup_perlin_sphere();
	void setup_quads();
	void setup_simple_light();
	void setup_two_lights();
	void setup_cornell_box();
	void setup_boxes();
	void setup_boxes_rotated();
	void setup_cornell_smoke();
	void setup_random_spheres_animated();
	void setup_simple_2d_parallel_test();
	void setup_final_scene();

	// file readers
	void setup_3d_obj();
	void setup_3d_obj_parallel();
	void setup_gltf();


	void set_bvh();


	// special effects
	void add_floor_mat(color& floor_color_, int size_factor_);
	void add_diffuse_light(color& light_color, int size_factor_);
	void add_fog(double& fog_density, color& fog_color);
	void change_material(std::string name_, std::unique_ptr<material> mat_);

};