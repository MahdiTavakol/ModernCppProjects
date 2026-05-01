#include "scene_factory.hpp"

#include "../Shared/rtweekend.h"

#include "obj_model_reader.h"
#include "obj_model_reader_parallel.h"
#include "../Geometry/aabb.h"
#include "../Algorithms/bvh.h"
#include "../Algorithms/camera.h"
#include "../Types/color.h"
#include "../Data/color_array.h"
#include "../Geometry/constant_medium.h"
#include "../Geometry/hittable.h"
#include "../Algorithms/hittable_list.h"
#include "../Input/input.h"
#include "../Geometry/interval.h"
#include "../Materials/material.h"
#include "../Algorithms/path.h"
#include "../Algorithms/communicator.h"
#include "../Geometry/quad.h"
#include "../Types/ray.h"
#include "../Algorithms/rtw_stb_image.h"
#include "../Geometry/planes.h"
#include "../Geometry/perlin.h"
#include "../Geometry/sphere.h"
#include "../Geometry/texture.h"
#include "../Types/vec3.h"
#include "../Output/output.h"

scene_factory::scene_factory(settings* wld_settings_, communicator* para_):
	mode{wld_settings_->return_mode()}, para{para_}
{
	// checking the setting type
	scene_settings* sett = dynamic_cast<scene_settings*>(wld_settings_);
	if (!sett)
		throw std::invalid_argument("Wrong settings object");

	obj_file_name = sett->return_obj_file_name();
	mtl_file_name = sett->return_mtl_file_name();
}

scene_factory::scene_factory(int mode_, std::unique_ptr<communicator>& para_) :
	mode{ mode_ }, para{para_.get()}
{}

scene_factory::scene_factory(int mode_, std::unique_ptr<communicator>& para_,
	                         std::string obj_file_name_):
	mode{mode_}, para{para_.get()}, 
	obj_file_name{obj_file_name_}
{ }

void scene_factory::create()
{
	world = std::make_unique<hittable_list>();


	switch (mode)
	{
	case RANDOM_SPHERES:
		setup_random_spheres();
		break;
	case CHECKER_BOARDS:
		setup_checker_boards();
		break;
	case EARTH_SPHERE:
		setup_earth_sphere();
		break;
	case PERLIN_SPHERE:
		setup_perlin_sphere();
		break;
	case QUADS:
		setup_quads();
		break;
	case SIMPLE_LIGHT:
		setup_simple_light();
		break;
	case TWO_LIGHTS:
		setup_two_lights();
		break;
	case CORNELL_BOX:
		setup_cornell_box();
		break;
	case TWO_BOXES:
		setup_boxes();
		break;
	case TWO_BOXES_ROTATED:
		setup_boxes_rotated();
		break;
	case CORNELL_SMOKE:
		setup_cornell_smoke();
		break;
	case OBJ_MODEL:
		setup_3d_obj();
		break;
	case OBJ_MODEL_PARALLEL:
		setup_3d_obj_parallel();
		break;
	case RANDOM_SPHERES_ANIMATED:
		setup_random_spheres_animated();
		break;
	case SIMPLE_2D_PARALEL_TEST:
		setup_simple_2d_parallel_test();
		break;
	}
}

std::unique_ptr<hittable_list> scene_factory::return_object()
{
	if (world == nullptr)
	{
		std::cout << "Warning: the world has already been returned" <<
			std::endl << "Calling the create method again!" << std::endl;
		this->create();
	}
	return std::move(world);
}

void scene_factory::setup_random_spheres()
{

	auto checker = std::make_unique<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	std::unique_ptr<material> mat = std::make_unique<lambertian>(std::move(checker));
	world->add(std::make_unique<sphere>(point3(0, -1000, 0), 1000, std::move(mat)));

	for (int a = -11; a < 11; a++)
		for (int b = -11; b < 11; b++)
		{
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				std::unique_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = std::make_unique<lambertian>(albedo);
					auto center2 = center + vec3(0, random_double(0, 0.5), 0);
					world->add(std::make_unique<sphere>(center, center2, 0.2, std::move(sphere_material)));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_unique<metal>(albedo, fuzz);
					world->add(std::make_unique<sphere>(center, 0.2, std::move(sphere_material)));
				}
				else {
					// glass
					sphere_material = std::make_unique<dielectric>(1.5);
					world->add(std::make_unique<sphere>(center, 0.2, std::move(sphere_material)));
				}
			}

		}

	auto material1 = std::make_unique<dielectric>(1.5);
	world->add(std::make_unique<sphere>(point3(0, 1, 0), 1.0, std::move(material1)));

	auto material2 = std::make_unique<lambertian>(color(0.4, 0.2, 0.1));
	world->add(std::make_unique<sphere>(point3(-4, 1, 0), 1.0, std::move(material2)));

	auto material3 = std::make_unique<metal>(color(0.7, 0.6, 0.5), 0.0);
	world->add(std::make_unique<sphere>(point3(4, 1, 0), 1.0, std::move(material3)));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_checker_boards()
{
	auto checker1 = std::make_unique<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	auto checker2 = std::make_unique<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

	auto material1 = std::make_unique<lambertian>(std::move(checker1));
	auto material2 = std::make_unique<lambertian>(std::move(checker2));

	world->add(std::make_unique<sphere>(point3(0, -10, 0), 10, std::move(material1)));
	world->add(std::make_unique<sphere>(point3(0, 10, 0), 10, std::move(material2)));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_earth_sphere()
{
	auto earth_texture = std::make_unique<image_texture>("3840px-Blue_Marble_2002.png");
	auto earth_surface = std::make_unique<lambertian>(std::move(earth_texture));
	auto globe = std::make_unique<sphere>(point3(0, 0, 0), 2, std::move(earth_surface));

	world->add(std::move(globe));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_perlin_sphere()
{
	auto pertext1 = std::make_unique<noise_texture>(4);
	auto pertext2 = std::make_unique<noise_texture>(4);
	world->add(std::make_unique<sphere>(point3(0, -1000, 0), 1000, std::make_unique<lambertian>(std::move(pertext1))));
	world->add(std::make_unique<sphere>(point3(0, 2, 0), 2, std::make_unique<lambertian>(std::move(pertext2))));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_quads()
{
	auto left_red = std::make_unique<lambertian>(color(1.0, 0.2, 0.2));
	auto back_green = std::make_unique<lambertian>(color(0.2, 1.0, 0.2));
	auto right_blue = std::make_unique<lambertian>(color(0.2, 0.2, 1.0));
	auto upper_orange = std::make_unique<lambertian>(color(1.0, 0.5, 0.0));
	auto lower_teal = std::make_unique<lambertian>(color(0.2, 0.8, 0.8));

	world->add(std::make_unique<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), std::move(left_red)));
	world->add(std::make_unique<circle>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), std::move(back_green)));
	world->add(std::make_unique<triangle>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), std::move(right_blue)));
	world->add(std::make_unique<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), std::move(upper_orange)));
	world->add(std::make_unique<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), std::move(lower_teal)));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_simple_light()
{
	auto pertext1 = std::make_unique<noise_texture>(4);
	auto pertext2 = std::make_unique<noise_texture>(*pertext1);
	auto material1 = std::make_unique<lambertian>(std::move(pertext1));
	auto material2 = std::make_unique<lambertian>(std::move(pertext2));
	world->add(std::make_unique<sphere>(point3(0, -1000, 0), 1000,std::move(material1)));
	world->add(std::make_unique<sphere>(point3(0, 2, 0), 2, std::move(material2)));

	auto difflight = std::make_unique<diffuse_light>(color(4, 4, 4));
	world->add(std::make_unique<quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), std::move(difflight)));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_two_lights()
{
	this->setup_simple_light();
	auto difflight = std::make_unique<diffuse_light>(color(4, 4, 4));
	world->add(std::make_unique<sphere>(point3(0, 7, 0), 2, std::move(difflight)));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_cornell_box()
{
	std::unique_ptr<material> red = std::make_unique<lambertian>(color(0.64, 0.05, 0.05));
	std::unique_ptr<material> white1 = std::make_unique<lambertian>(color(0.73, 0.73, 0.73));
	std::unique_ptr<material> white2 = std::make_unique<lambertian>(color(0.73, 0.73, 0.73));
	std::unique_ptr<material> green = std::make_unique<lambertian>(color(0.12, 0.45, 0.15));
	std::unique_ptr<material> light = std::make_unique<diffuse_light>(color(15, 15, 15));

	world->add(std::make_unique<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), std::move(green)));
	world->add(std::make_unique<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), std::move(red)));
	world->add(std::make_unique<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), std::move(light)));
	world->add(std::make_unique<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), std::move(white1)));
	world->add(std::make_unique<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), std::move(white2)));


	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_boxes()
{
	setup_cornell_box();


	std::vector < std::unique_ptr<material>> matVec1, matVec2;
	for (int i = 0; i < 6; i++) {
		matVec1.push_back(std::make_unique<lambertian>(color(0.73, 0.73, 0.73)));
		matVec2.push_back(std::make_unique<lambertian>(color(0.73, 0.73, 0.73)));
	}


	std::unique_ptr<hittable> box1 =
		box(point3(0, 0, 0), point3(165, 330, 165), matVec1);
	std::unique_ptr<hittable> box2 =
		box(point3(0, 0, 0), point3(165, 165, 165), matVec2);

	world->add(std::move(box1));
	world->add(std::move(box2));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_boxes_rotated()
{
	setup_cornell_box();
	std::vector < std::unique_ptr<material>> matVec1, matVec2;
	for (int i = 0; i < 6; i++) {
		matVec1.push_back(std::make_unique<lambertian>(color(0.73, 0.73, 0.73)));
		matVec2.push_back(std::make_unique<lambertian>(color(0.73, 0.73, 0.73)));
	}


	std::unique_ptr<hittable> box1 =
		box(point3(0, 0, 0), point3(165, 330, 165), matVec1);
	std::unique_ptr<hittable> box2 =
		box(point3(0, 0, 0), point3(165, 165, 165), matVec2);

	box1 = std::make_unique<rotate_y>(std::move(box1), 15);
	box1 = std::make_unique<translate>(std::move(box1), vec3(265, 0, 295));
	world->add(std::move(box1));

	box2 = std::make_unique<rotate_y>(std::move(box2), -18);
	box2 = std::make_unique<translate>(std::move(box2), vec3(130, 0, 65));
	world->add(std::move(box2));


	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));

}

void scene_factory::setup_cornell_smoke()
{
	setup_cornell_box();

	std::vector < std::unique_ptr<material>> matVec1, matVec2;
	for (int i = 0; i < 6; i++) {
		matVec1.push_back(std::make_unique<lambertian>(color(0.73, 0.73, 0.73)));
		matVec2.push_back(std::make_unique<lambertian>(color(0.73, 0.73, 0.73)));
	}


	std::unique_ptr<hittable> box1 =
		box(point3(0, 0, 0), point3(165, 330, 165), matVec1);
	std::unique_ptr<hittable> box2 =
		box(point3(0, 0, 0), point3(165, 165, 165), matVec2);



	//box1 = std::make_unique<rotate_y>(std::move(box1), 15);
	//box1 = std::make_unique<translate>(std::move(box1), vec3(265, 0, 295));



	//box2 = std::make_unique<rotate_y>(std::move(box2), -18);
	//box2 = std::make_unique<translate>(std::move(box2), vec3(130, 0, 65));



	world->add(std::make_unique<constant_medium>(std::move(box1), 0.01, color(0, 0, 0)));
	world->add(std::make_unique<constant_medium>(std::move(box2), 0.01, color(1, 1, 1)));


	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}


void scene_factory::setup_3d_obj()
{
	std::unique_ptr<obj_model_reader> model_reader =
		std::make_unique<obj_model_reader>(obj_file_name, para);
	model_reader->read();
	world = model_reader->return_world();
	//auto bvh = std::make_unique<bvh_node>(std::move(world));
	//world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_3d_obj_parallel()
{
	world = std::make_unique<hittable_list_parallel>();
	std::unique_ptr<obj_model_reader_parallel> model_reader =
		std::make_unique<obj_model_reader_parallel>(obj_file_name,para);
	model_reader->read();
	world = model_reader->return_world();

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_random_spheres_animated()
{
	// Creating the spheres 
	setup_random_spheres();
}

void scene_factory::setup_simple_2d_parallel_test()
{
	point3 min{ -70, -70, -30 }, max{70, 70, 30 };
	point3 delta{ 5,5,5 };
	auto size_config = para->return_size_config();
	auto rank_config = para->return_rank_config();
	std::vector<std::pair<point3, point3>> rank_bounds;

	point3 per_rank_dim;
	per_rank_dim[0] = (max.x() - min.x()) / size_config[0];
	per_rank_dim[1] = (max.y() - min.y()) / size_config[1];
	per_rank_dim[2] = (max.z() - min.z());


	for (int i = 0; i < size_config[0]; i++)
	{
		for (int j = 0; j < size_config[1]; j++)
		{
			point3 rank_min, rank_max;
			rank_min = min + point3(i * per_rank_dim.x(), j * per_rank_dim.y(), 0);
			rank_max = rank_min + point3(per_rank_dim.x(), per_rank_dim.y(), 0);
			rank_min = rank_min + delta;
			rank_max = rank_max - delta;
			rank_bounds.push_back({ rank_min, rank_max });
		}
	}


	int count = 0;
	for (auto& rank_bound : rank_bounds)
	{
		std::vector < std::unique_ptr<material>> matVec;
		double r = static_cast<double>((count * 70) % 256)/256.0;
		double g = static_cast<double>((count * 150) % 256)/256.0;
		double b = static_cast<double>((count * 230) % 256)/256.0;
		count++;
		color myColor = color{ r,g,b };
		for (int i = 0; i < 6; i++) {
			matVec.push_back(std::make_unique<lambertian>(myColor));
		}
		std::unique_ptr<hittable> box1 =
			box(rank_bound.first, rank_bound.second, matVec);
		world->add(std::move(box1));
	}
	//auto bvh = std::make_unique<bvh_node>(std::move(world));
	//world = std::make_unique<hittable_list>(std::move(bvh));
}
