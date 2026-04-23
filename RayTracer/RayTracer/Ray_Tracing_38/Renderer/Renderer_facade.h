#pragma once

#include <memory>

#include "../Shared/rtweekend.h"

#include "../Input/obj_model_reader.h"
#include "../Input/obj_model_reader_parallel.h"
#include "../Algorithms/camera.h"
#include "../Algorithms/camera_settings.h"
#include "../Algorithms/settings.h"
#include "../Input/scene_factory.h"
#include "../Geometry/hittable.h"
#include "../Algorithms/hittable_list.h"
#include "../Input/input.h"
#include "../Algorithms/communicator.h"
#include "../Types/vec3.h"
#include "../Output/output.h"
#include "Renderer.h"
#include "Renderer_Animation.h"


class renderer_facade
{
public:
	renderer_facade(int argc, char** argv, int _mode, std::string _filename,
		std::array<int, 2> size_config_, MPI_Comm comm_ = MPI_COMM_WORLD);
	virtual ~renderer_facade();
	virtual void setup();
	void add(std::unique_ptr<hittable>& object);
	virtual void render();
	void write_file();



protected:
	int mode;
	

	std::string obj_file_name = "../../models/Toyota_Sequoia_2023/Toyota_Sequoia_2023_2015_obj.obj";
	std::string filename;


	std::unique_ptr<renderer> rendererObj;
	std::unique_ptr<scene_factory> world_factory;
	std::unique_ptr<settings> settingsObj;
	std::unique_ptr<camera> cam;
	std::unique_ptr<communicator> para;
	std::unique_ptr<input> in;
	std::unique_ptr<hittable_list> world;
	std::unique_ptr<output> writer;
	point3 camera_location;
};