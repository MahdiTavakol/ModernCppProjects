#include "Renderer_facade.h"

#include "../Input/communicator_settings.h"


renderer_facade::renderer_facade(int argc, char** argv, int mode_,
	MPI_Comm comm_)
{
	// the factory object
	builder = std::make_unique<factory>(argc, argv, mode_, comm_);
}



renderer_facade::~renderer_facade()
{
}

void renderer_facade::setup()
{
	// creating the objects
	builder->create();
	// returning each object
	rend = builder->return_renderer();
	cam = builder->return_camera();
	para = builder->return_comm();
	writer = builder->return_writer();
	world = builder->return_world();
}


void renderer_facade::add(std::unique_ptr<hittable>& object)
{
	world->add(std::move(object));
}

void renderer_facade::render()
{
	rend->render(cam.get(), writer.get(), world.get());
}


void renderer_facade::write()
{
	// writing the file - it is the job of the writer to just write the file in the rank 0, so no need to check the rank here.
	// also the renderer sets the filename in its render method, so the writer will always write the file with the correct name.
	// also the renderer is in the charge of updating the file contents in its render method, 
	// so the writer just needs to write the file with the current contents.
	writer->write_file();
}



