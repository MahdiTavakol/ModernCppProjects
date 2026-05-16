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
	mode{wld_settings_->return_mode()}, para{para_}, list{std::make_unique<material_list>()}
{
	// checking the setting type
	stngs = dynamic_cast<scene_settings*>(wld_settings_);
	if (!stngs)
		throw std::invalid_argument("Wrong settings object");


	obj_file_name = stngs->return_obj_file_name();
	mtl_file_name = stngs->return_mtl_file_name();
}

scene_factory::scene_factory(int mode_, std::unique_ptr<communicator>& para_) :
	mode{ mode_ }, para{para_.get()}, list{ std::make_unique<material_list>() }
{}

scene_factory::scene_factory(int mode_, std::unique_ptr<communicator>& para_,
	                         std::string obj_file_name_):
	mode{mode_}, para{para_.get()}, 
	obj_file_name{obj_file_name_},
	list{ std::make_unique<material_list>() }
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


	// adding special effects
	if (stngs->specialCheck(specialEnum::SCALE))
	{
		bool set = false;
		double scale_factor;
		vec3 center = world->com("main", set);
		if (set == false)
			throw std::invalid_argument("There is nothing here to be scaled!");
		stngs->scale_settings(scale_factor);
		world->scale(center, scale_factor);
	}
	if (stngs->specialCheck(specialEnum::FLOOR))
	{
		color floor_color;
		double floor_size;
		stngs->floor_settings(floor_color, floor_size);
		add_floor_mat(floor_color, floor_size);
	}
	if (stngs->specialCheck(specialEnum::DIFFUSE_LIGHT))
	{
		color light_color;
		double size_factor;
		stngs->light_settings(light_color, size_factor);
		add_diffuse_light(light_color, size_factor);
	}
	if (stngs->specialCheck(specialEnum::FOG))
	{
		color fog_color;
		double fog_density;
		stngs->fog_settings(fog_density, fog_color);
		add_fog(fog_density, fog_color);
	}

	set_bvh();
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

std::unique_ptr<material_list> scene_factory::return_mtl_list()
{
	if (list == nullptr)
	{
		std::cout << "Warning: the material_list has already been returned" <<
			std::endl << "Calling the create method again!" << std::endl;
		this->create();
	}
	return std::move(list);
}


void scene_factory::setup_random_spheres()
{
	int mat_indx = 0;

	auto checker = std::make_unique<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	std::unique_ptr<material> mat = std::make_unique<lambertian>(std::move(checker));
	mat_indx = list->push_back("checker", std::move(mat));
	world->add(std::make_unique<sphere>(point3(0, -1000, 0), 1000,mat_indx));

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
					std::string mat_name = "diffuse-" 
						+ std::to_string(albedo[0]) + 
						"-" + std::to_string(albedo[1]) + 
						"-" + std::to_string(albedo[2]);
					mat_indx = list->push_back(mat_name, std::move(sphere_material));
					auto center2 = center + vec3(0, random_double(0, 0.5), 0);
					world->add(std::make_unique<sphere>(center, center2, 0.2, mat_indx));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					std::string mat_name = "metal-"
						+ std::to_string(albedo[0]) +
						"-" + std::to_string(albedo[1]) +
						"-" + std::to_string(albedo[2]) + 
						"--" + std::to_string(fuzz);
					mat_indx = list->push_back(mat_name, std::move(sphere_material));
					sphere_material = std::make_unique<metal>(albedo, fuzz);
					world->add(std::make_unique<sphere>(center, 0.2, mat_indx));
				}
				else {
					// glass
					sphere_material = std::make_unique<dielectric>(1.5);
					std::string mat_name = "dielectric-1.5";
					mat_indx = list->push_back(mat_name, std::move(sphere_material));
					world->add(std::make_unique<sphere>(center, 0.2, mat_indx));
				}
			}

		}

	auto material1 = std::make_unique<dielectric>(1.5);
	std::string mat_name = "dielectric-1.5";
	mat_indx = list->push_back(mat_name, std::move(material1));
	world->add(std::make_unique<sphere>(point3(0, 1, 0), 1.0, mat_indx));

	auto material2 = std::make_unique<lambertian>(color(0.4, 0.2, 0.1));
	mat_name = "lambertian";
	mat_indx = list->push_back(mat_name, std::move(material2));
	world->add(std::make_unique<sphere>(point3(-4, 1, 0), 1.0, mat_indx));

	auto material3 = std::make_unique<metal>(color(0.7, 0.6, 0.5), 0.0);
	mat_name = "metal";
	mat_indx = list->push_back(mat_name, std::move(material3));
	world->add(std::make_unique<sphere>(point3(4, 1, 0), 1.0, mat_indx));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_checker_boards()
{
	int mat_indx = 0;
	std::string mat_name;

	auto checker1 = std::make_unique<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	auto checker2 = std::make_unique<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

	auto material1 = std::make_unique<lambertian>(std::move(checker1));
	auto material2 = std::make_unique<lambertian>(std::move(checker2));


	mat_name = "material1";
	mat_indx = list->push_back(mat_name, std::move(material1));
	world->add(std::make_unique<sphere>(point3(0, -10, 0), 10, mat_indx));

	mat_name = "material2";
	mat_indx = list->push_back(mat_name, std::move(material2));
	world->add(std::make_unique<sphere>(point3(0, 10, 0), 10, mat_indx));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_earth_sphere()
{
	int mat_indx = 0;
	std::string mat_name;

	auto earth_texture = std::make_unique<image_texture>("3840px-Blue_Marble_2002.png");


	auto earth_surface = std::make_unique<lambertian>(std::move(earth_texture));
	mat_name = "material1";
	mat_indx = list->push_back(mat_name, std::move(earth_surface));
	auto globe = std::make_unique<sphere>(point3(0, 0, 0), 2, mat_indx);

	world->add(std::move(globe));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_perlin_sphere()
{
	int mat_indx = 0;
	std::string mat_name;

	auto pertext1 = std::make_unique<noise_texture>(4);
	mat_name = "material1";
	auto material1 = std::make_unique<lambertian>(std::move(pertext1));
	mat_indx = list->push_back(mat_name, std::move(material1));
	world->add(std::make_unique<sphere>(point3(0, -1000, 0), 1000, mat_indx));
	
	
	auto pertext2 = std::make_unique<noise_texture>(4);
	mat_name = "material2";
	auto material2 = std::make_unique<lambertian>(std::move(pertext2));
	mat_indx = list->push_back(mat_name, std::move(material2));
	world->add(std::make_unique<sphere>(point3(0, 2, 0), 2, mat_indx));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_quads()
{
	int mat_indx = 0;
	std::string mat_name;

	struct
	{
		std::string title;
		color mat_color;
		point3 a;
		vec3 b, c;
	} infos[5] = {
		{"left_red",color(1.0, 0.2, 0.2),point3(-4, -4, 5), vec3(0, 0, -8), vec3(0, 8, 0)},
		{"back_green",color(0.2, 1.0, 0.2),point3(-4, -4, 0), vec3(8, 0, 0), vec3(0, 8, 0)},
		{"right_blue",color(0.2, 0.2, 1.0),point3(4, -4, 0), vec3(0, 0, 8), vec3(0, 8, 0)},
		{"upper_orange",color(1.0, 0.5, 0.0),point3(-4, 4, 0), vec3(8, 0, 0), vec3(0, 0, 8)},
		{"lower_teal",color(0.2, 0.8, 0.8),point3(-4, -4, 5), vec3(8, 0, 0), vec3(0, 0, -8)}
	};

	for (auto& info : infos)
	{
		auto mat = std::make_unique<lambertian>(info.mat_color);
		mat_name = info.title;
		mat_indx = list->push_back(mat_name, std::move(mat));
		world->add(std::make_unique<quad>(info.a, info.b, info.c, mat_indx));
	}

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_simple_light()
{
	int mat_indx = 0;
	std::string mat_name;

	auto pertext1 = std::make_unique<noise_texture>(4);
	auto material1 = std::make_unique<lambertian>(std::move(pertext1));
	mat_name = "material1";
	mat_indx = list->push_back(mat_name, std::move(material1));
	world->add(std::make_unique<sphere>(point3(0, -1000, 0), 1000, mat_indx));

	
	auto pertext2 = std::make_unique<noise_texture>(4);
	auto material2 = std::make_unique<lambertian>(std::move(pertext2));
	mat_name = "material2";
	mat_indx = list->push_back(mat_name, std::move(material2));
	world->add(std::make_unique<sphere>(point3(0, 2, 0), 2, mat_indx));



	auto difflight = std::make_unique<diffuse_light>(color(4, 4, 4));
	mat_name = "diffuse";
	mat_indx = list->push_back(mat_name, std::move(difflight));
	world->add(std::make_unique<quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), mat_indx));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_two_lights()
{
	std::string mat_name;
	int mat_indx;

	this->setup_simple_light();
	auto difflight = std::make_unique<diffuse_light>(color(4, 4, 4));
	mat_name = "diffuse";
	mat_indx = list->push_back(mat_name, std::move(difflight));
	world->add(std::make_unique<sphere>(point3(0, 7, 0), 2, mat_indx));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_cornell_box()
{
	std::unique_ptr<material> red = std::make_unique<lambertian>(color(0.64, 0.05, 0.05));
	std::unique_ptr<material> white = std::make_unique<lambertian>(color(0.73, 0.73, 0.73));
	std::unique_ptr<material> green = std::make_unique<lambertian>(color(0.12, 0.45, 0.15));
	std::unique_ptr<material> light = std::make_unique<diffuse_light>(color(15, 15, 15));

	int red_mat_indx = list->push_back("red", std::move(red));
	int white_mat_indx = list->push_back("white", std::move(white));
	int green_mat_indx = list->push_back("green", std::move(green));
	int light_mat_indx = list->push_back("light", std::move(light));


	double size = 556;
	world->add(std::make_unique<quad>(point3(size, 0, 0), vec3(0, size, 0), vec3(0, 0, size), green_mat_indx));
	world->add(std::make_unique<quad>(point3(0, 0, 0), vec3(0, size, 0), vec3(0, 0, size), red_mat_indx));
	world->add(std::make_unique<quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light_mat_indx));
	world->add(std::make_unique<quad>(point3(size, size, size), vec3(-size, 0, 0), vec3(0, 0, -size), white_mat_indx));
	world->add(std::make_unique<quad>(point3(0, 0, size), vec3(size, 0, 0), vec3(0, size, 0), white_mat_indx));


	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_boxes()
{
	setup_cornell_box();


	std::unique_ptr<material> mat;
	mat = std::make_unique<lambertian>(color(0.73, 0.73, 0.73));
	int mat_indx = list->push_back("lambertian", std::move(mat));



	std::unique_ptr<hittable> box1 =
		box(point3(0, 0, 0), point3(165, 330, 165), mat_indx);
	std::unique_ptr<hittable> box2 =
		box(point3(0, 0, 0), point3(165, 165, 165), mat_indx);

	world->add(std::move(box1));
	world->add(std::move(box2));

	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}

void scene_factory::setup_boxes_rotated()
{
	setup_cornell_box();

	std::unique_ptr<material> mat;
	mat = std::make_unique<lambertian>(color(0.73, 0.73, 0.73));
	int mat_indx = list->push_back("lambertian", std::move(mat));


	std::unique_ptr<hittable> box1 =
		box(point3(0, 0, 0), point3(165, 330, 165), mat_indx);
	std::unique_ptr<hittable> box2 =
		box(point3(0, 0, 0), point3(165, 165, 165), mat_indx);

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

	std::unique_ptr<material> mat;
	mat = std::make_unique<lambertian>(color(0.73, 0.73, 0.73));
	int mat_indx = list->push_back("lambertian", std::move(mat));



	std::unique_ptr<hittable> box1 =
		box(point3(0, 0, 0), point3(165, 330, 165), mat_indx);
	std::unique_ptr<hittable> box2 =
		box(point3(0, 0, 0), point3(165, 165, 165), mat_indx);



	//box1 = std::make_unique<rotate_y>(std::move(box1), 15);
	//box1 = std::make_unique<translate>(std::move(box1), vec3(265, 0, 295));



	//box2 = std::make_unique<rotate_y>(std::move(box2), -18);
	//box2 = std::make_unique<translate>(std::move(box2), vec3(130, 0, 65));



	world->add(std::make_unique<constant_medium>(std::move(box1), 0.01, color(0, 0, 0),*list));
	world->add(std::make_unique<constant_medium>(std::move(box2), 0.01, color(1, 1, 1),*list));


	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}


void scene_factory::setup_3d_obj()
{
	std::unique_ptr<obj_model_reader> model_reader =
		std::make_unique<obj_model_reader>(obj_file_name, mtl_file_name, para);
	model_reader->read();
	world = model_reader->return_world();
	list = model_reader->return_mtl_list();
}

void scene_factory::setup_3d_obj_parallel()
{
	world = std::make_unique<hittable_list_parallel>();
	std::unique_ptr<obj_model_reader_parallel> model_reader =
		std::make_unique<obj_model_reader_parallel>(obj_file_name,para);
	model_reader->read();
	world = model_reader->return_world();
	list = model_reader->return_mtl_list();
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

void scene_factory::add_floor_mat(color& floor_color_, int size_factor_)
{
	bool set = false;
	aabb bbox = world->bounding_box("main", set);

	vec3 com = world->com();
	double xLength = bbox.axis_interval(0).size();
	double yLength = bbox.axis_interval(1).size();
	double zLength = bbox.axis_interval(2).size();
	vec3 min = vec3(bbox.axis_interval(0).min, bbox.axis_interval(1).min, bbox.axis_interval(2).min);


	double diag = std::sqrt(
		xLength * xLength +
		yLength * yLength +
		zLength * zLength
	);


	double floor_size = size_factor_ * diag;
	double y_floor = min[1] - 0.02 * diag;

	std::unique_ptr<material> floor_mat = std::make_unique<lambertian>(floor_color_);
	int red_mat_indx = list->push_back("floor", std::move(floor_mat));
	world->add(std::make_unique<quad>(
		point3(com[0] - floor_size / 2.0, y_floor, com[2] - floor_size / 2.0),
		vec3(floor_size, 0, 0),
		vec3(0, 0, floor_size),
		red_mat_indx));
}

void scene_factory::add_diffuse_light(color& light_color_, int size_factor_)
{
	bool set = false;
	aabb bbox = world->bounding_box("main", set);


	vec3 com = world->com();
	double xLength = bbox.axis_interval(0).size();
	double yLength = bbox.axis_interval(1).size();
	double zLength = bbox.axis_interval(2).size();
	vec3 min = vec3(bbox.axis_interval(0).min, bbox.axis_interval(1).min, bbox.axis_interval(2).min);


	double diag = std::sqrt(
		xLength * xLength +
		yLength * yLength +
		zLength * zLength
	);

	vec3 light_location = com + vec3(diag, size_factor_ * diag, size_factor_ * diag);
	double light_size = 0.5 * diag;

	auto difflight = std::make_unique<diffuse_light>(light_color_);
	std::string mat_name = "diffuse";
	int mat_indx = list->push_back(mat_name, std::move(difflight));
	world->add(std::make_unique<sphere>(light_location, light_size, mat_indx));
}

void scene_factory::add_fog(double& fog_density_, color& fog_color_)
{
	std::unique_ptr<hittable> fog = 
		std::make_unique<constant_medium>(std::move(world), fog_density_, fog_color_, *list);
	world = std::make_unique<hittable_list>(std::move(world));
}

void scene_factory::set_bvh()
{
	auto bvh = std::make_unique<bvh_node>(std::move(world));
	world = std::make_unique<hittable_list>(std::move(bvh));
}
