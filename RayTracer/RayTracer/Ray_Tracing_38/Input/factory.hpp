#pragma once
#include "../Renderer/Renderer.h"
#include "scene_factory.hpp"
#include "renderer_factory.hpp"
#include "app_settings.h"
#include "../Algorithms/camera.h"
#include "../Algorithms/communicator.h"
#include "input.h"
#include "../Algorithms/hittable_list.h"
#include "../Output/output.h"
#include <mpi.h>

class factory
{
public:
	factory(int argc, char** argv, int mode_,
		MPI_Comm comm_);
	void create();
	std::unique_ptr<communicator> return_comm();
	std::unique_ptr<renderer> return_renderer();
	std::unique_ptr<camera> return_camera();
	std::unique_ptr<output> return_writer();
	std::unique_ptr<hittable_list> return_world();
protected:
	// the mode used for various default parameters
	int mode;
	// special objects which are created before the input class
	std::unique_ptr<communicator> para;
	// the parser 
	std::unique_ptr<input> in;
	// the settings array for all the objects
	std::unique_ptr<app_settings> stngs;
	// objects without dedicated factories
	std::unique_ptr<camera> cam;
	std::unique_ptr<output> writer;
	// objects with special factories
	// we will call their return method so 
	// we do not need to keep a unique_ptr of those types
	// renderer factory
	std::unique_ptr<renderer_factory> rend_factory;
	// hittable_list factory
	std::unique_ptr<scene_factory> world_factory;
	// the settings map
	std::map<std::string, int> app_set_map =
	{
		{"scene",0},
		{"camera",1},
		{"image",2},
		{"output",3},
		{"renderer",4}
	};

};