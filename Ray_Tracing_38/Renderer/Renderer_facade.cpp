#include "Renderer_facade.h"
#include "../Renderer/Renderer_Async.h"
#include "../Output/output_async.h"

#include "../Input/communicator_settings.h"

#include <thread>
#include <chrono>


renderer_facade::renderer_facade(int argc, char** argv, int mode_,
	MPI_Comm comm_):
	mode{mode_}, comm{comm_}
{
	// the factory object
	builder = std::make_unique<factory>(argc, argv, mode_, comm_);
	// copying the argv to be used by the profiler
	argv_vec.reserve(argc);
	for (int i = 0; i < argc; i++)
	{
		argv_vec.push_back(std::string(argv[i]));
	}
	ptr = new renderer_facade_inputs;
	ptr->argv_vec = &argv_vec;

}



renderer_facade::~renderer_facade()
{
	delete ptr;
}

void renderer_facade::setup()
{
	// checking if profiling is done or not
	builder->return_profiling_info(profiling, rend, timers);
	// this objects are created in the c'tor of the factory class
	// so they can be returned before calling the create() function
	// of the factory class
	 
	// just returning the timer
	timer = builder->return_timer();
	// getting the error pointer
	error = builder->return_error();
	// normal execution
	if (!profiling) {
		// creating the objects
		builder->create();
		// returning each object
		rend = builder->return_renderer();
		cam = builder->return_camera();
		para = builder->return_comm();
		writer = builder->return_writer();
		world = builder->return_world();
		mtl_list = builder->return_mtl_list();
		img = builder->return_image();
		// objects with specific setup methods
		writer->setup(img.get());
		rend->setup(world.get(), mtl_list.get());
	}
	else
	{
		// resting the logger class
		error->reset_mode(print_mode::PROFILING);
		ptr->argv_vec = &argv_vec;
		ptr->mode = mode;
		ptr->comm = comm;
		rend->set_resource_pointers(std::move(timers), ptr);
	}
}


void renderer_facade::add(std::unique_ptr<hittable>& object)
{
	world->add(std::move(object));
}

void renderer_facade::render()
{
	timer->start_event("rendering");
 	rend->render(img.get(),cam.get(), writer.get());
	timer->stop_event("rendering");
}


void renderer_facade::write()
{
	// writing the file - it is the job of the writer to just write the file in the rank 0, so no need to check the rank here.
	// also the renderer sets the filename in its render method, so the writer will always write the file with the correct name.
	// also the renderer is in the charge of updating the file contents in its render method, 
	// so the writer just needs to write the file with the current contents.
	timer->start_event("output");
	rend->write_file(writer.get(),img.get());
	timer->stop_event("output");
}

void renderer_facade::print_timing_info()
{
	rend->print_timing_info();
}

std::unique_ptr<Logger> renderer_facade::return_error()
{
	return std::move(error);
}



