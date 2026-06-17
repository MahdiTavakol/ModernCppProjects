#pragma once

#include <memory>
#include <mpi.h>

#include "Renderer.h"
#include "../Algorithms/camera.h"
#include "../Algorithms/communicator.h"
#include "../Output/output.h"
#include "../Algorithms/hittable_list.h"
#include "../Input/factory.hpp"
#include "../Materials/material_list.h"
#include "../Output/Logger.h"
#include "../Output/profiler.h"


class renderer_facade
{
public:
	renderer_facade(int argc, char** argv, int _mode, MPI_Comm comm_ = MPI_COMM_WORLD);
	virtual ~renderer_facade();
	virtual void setup();
	void add(std::unique_ptr<hittable>& object);
	virtual void render();
	void write();
	void print_timing_info();
	std::unique_ptr<Logger> return_error();



protected:
	// the factory object
	std::unique_ptr<factory> builder;

	// a pointer to input arguments (needed by the profiler)
	renderer_facade_inputs* ptr;
	// resources
	std::unique_ptr<Logger> error;
	std::unique_ptr<image> img;
	std::unique_ptr<renderer> rend;
	std::unique_ptr<camera> cam;
	std::unique_ptr<communicator> para;
	std::unique_ptr<output> writer;
	std::unique_ptr<hittable_list> world;
	std::unique_ptr<material_list> mtl_list;
	std::unique_ptr<profiler> timer;

	// profiling resources
	bool profiling = false;
	int nProfilings = 1;
	std::unique_ptr<renderer> profiling_renderer;
	std::vector<std::unique_ptr<profiler>> timers;
	// args need for the profiler
	std::vector<std::string> argv_vec;

	MPI_Comm comm;
	int mode;

};