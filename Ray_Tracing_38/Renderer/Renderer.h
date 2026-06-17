#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <string>

#include "../Algorithms/communicator.h"
#include "../Algorithms/path.h"
#include "../Algorithms/camera.h"
#include "../Output/output.h"
#include "../Algorithms/hittable_list.h"
#include "../Materials/material_list.h"
#include "../Output/profiler.h"
#include <mpi.h>


struct renderer_facade_inputs
{
	std::vector<std::string>* argv_vec;
	int mode;
	MPI_Comm comm;
};

class renderer
{
public:
	renderer(
		communicator* para_,
		Logger* error_,
		profiler* timer_,
		std::unique_ptr<path>&& pth_,
		std::string info_ = "", 
		bool verbose_ = false);
	virtual ~renderer();
	virtual void setup(hittable_list* world_, material_list* mtls_);
	virtual void set_resource_pointers(
		std::vector<std::unique_ptr<profiler>>&& timers_,
		renderer_facade_inputs* ptrs_) {}
	virtual void render(image* img_, camera* cam_, output* writer_);
	virtual void write_file(output* writer_, image *img_);
	virtual void print_timing_info();
	
	

protected:
	std::string info = "";
	bool verbose = false;
	// shared resources of the program
	communicator* para;
	Logger* error;
	profiler* timer;

	// camera path
	std::unique_ptr<path> pth;

	// filename
	std::string filename;


	// since the world and the materials
	// rarely change during rendering 
	// we put these two as properties of
	// the renderer class
	hittable_list* world;
	material_list* mtls;
};

#endif
