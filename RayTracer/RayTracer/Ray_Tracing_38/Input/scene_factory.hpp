#pragma once

#include "../Shared/rtweekend.h"


#include "../Algorithms/hittable_list.h"
#include "../Algorithms/communicator.h"
#include "scene_settings.h"

class scene_factory {
public:
	scene_factory(settings* wld_settings, communicator* para_);
	scene_factory(int mode_, std::unique_ptr<communicator>& para_);
	scene_factory(int mode_, std::unique_ptr<communicator>& para_,
		          std::string obj_file_name_);
	void create();
	std::unique_ptr<hittable_list> return_object();

protected:
	const int mode;
	std::unique_ptr<hittable_list> world;
	communicator* para;
	std::string obj_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_obj.obj";
	std::string mtl_file_name;

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
	void setup_3d_obj();
	void setup_3d_obj_parallel();
	void setup_random_spheres_animated();
	void setup_simple_2d_parallel_test();

};
