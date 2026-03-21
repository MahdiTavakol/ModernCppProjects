#pragma once

#include "../Shared/rtweekend.h"


#include "../Algorithms/hittable_list.h"
#include "../Algorithms/parallel.h"

class scene_factory {
public:
	scene_factory(int mode_, std::unique_ptr<parallel>& para_);
	scene_factory(int mode_, std::unique_ptr<parallel>& para_,
		          std::string obj_file_name_);
	void create();
	std::unique_ptr<hittable_list> return_world();

protected:
	const int mode;
	std::unique_ptr<hittable_list> world;
	parallel* para;
	std::string obj_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_obj.obj";

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

};
