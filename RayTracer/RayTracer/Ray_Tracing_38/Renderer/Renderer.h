#ifndef RENDERER_H
#define RENDERER_H
#include <memory>

#include "../Shared/rtweekend.h"

#include "../Input/obj_model_reader.h"
#include "../Input/obj_model_reader_parallel.h"
#include "../Geometry/aabb.h"
#include "../Algorithms/bvh.h"
#include "../Algorithms/camera.h"
#include "../Algorithms/camera_settings.h"
#include "../Input/scene_factory.h"
#include "../Algorithms/camera_derived.h"
#include "../Algorithms/camera_parallel.h"
#include "../Types/color.h"
#include "../Data/color_array.h"
#include "../Geometry/constant_medium.h"
#include "../Geometry/hittable.h"
#include "../Algorithms/hittable_list.h"
#include "../Input/input.h"
#include "../Geometry/interval.h"
#include "../Materials/material.h"
#include "../Algorithms/path.h"
#include "../Algorithms/parallel.h"
#include "../Algorithms/mpiParallel.h"
#include "../Geometry/quad.h"
#include "../Types/ray.h"
#include "../Algorithms/rtw_stb_image.h"
#include "../Geometry/planes.h"
#include "../Geometry/perlin.h"
#include "../Geometry/sphere.h"
#include "../Geometry/texture.h"
#include "../Types/vec3.h"
#include "../Output/output.h"
#include "../Output/output_serial.h"


class renderer
{
public:
	renderer(int argc, char** argv, int _mode, std::string _filename, 
			 std::array<int, 2> size_config_, MPI_Comm comm_ = MPI_COMM_WORLD);
	renderer(int argc, char** argv, int _mode, std::string _filename,
		std::string obj_file_, std::array<int, 2> size_config_,
			 MPI_Comm comm_ = MPI_COMM_WORLD);
	virtual ~renderer();
	virtual void setup();
	void add(std::unique_ptr<hittable>& object);
	virtual void render();
	void write_file();
	void update_c_array();
	
	

protected:
	int mode;
	std::string obj_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_obj.obj";
	std::string filename;
	std::unique_ptr<scene_factory> world_factory;
	std::unique_ptr<camera_settings> cam_settings;
	std::unique_ptr<camera> cam;
	std::unique_ptr<parallel> para;
	std::unique_ptr<input> in;
	std::unique_ptr<hittable_list> world;
	std::unique_ptr<output> writer;
	point3 camera_location;

	// since it is owned by the parallel object
	// here we should have raw pointer
	color_array* c_array;
};

#endif
