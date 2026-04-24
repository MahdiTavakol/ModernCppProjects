#include "Renderer_facade.h"


renderer_facade::renderer_facade(int argc, char** argv, int _mode,
	std::string _filename,
	std::array<int, 2> size_config_,
	MPI_Comm comm_)
	:
	mode{ _mode },
	filename{ _filename }
{
	/*
	* The object of the Input class
	* first initialize the app_settings object
	* with the default parameters for each class 
	* and then each parameters is overwritten 
	* based on the values read either from the 
	* input script or the CML.
	* 
	* 
	* The input class is solely in charge 
	* of the initializig various settings objects
	* so it is the only class that knows the implementation
	* of various settings classes. Thus, the app_settings
	* class cannot initialize these settings to decouple it from 
	* the settings implementation. Thus, the input is
	* the only class coupled to the settings classes implementations.
	*/

	// the parallel class in charge of the parallel communicator
	para = std::make_unique<mpiComm>(comm_, size_config_);
	// changing the camera settings based on the user input
	in = std::make_unique<input>(argc, argv, app_set_map, para.get());
	// getting the app_settings object from the in
	stngs = in->return_app_settings();
	// getting the settings for the scene_factory object
	auto& sett = *stngs;
	settings* scene_settings = sett["scene"];
	// the world_factory is in charge of lazy creation of the scene.
	world_factory = std::make_unique<scene_factory>(scene_settings, para.get());

	// think about how the run mode (animation vs static) 
	// and the size should be parsed from the argv, argc
	// May be argc, argv goes to a list of strings and
	// each parser takes its own args from the list and removes its from the list
	std::array<int, 2> size_config{ 4, 2 };
	// need to think about this part.. Ideally should be set in the scene factory.
	std::string obj_file = "../models/Simple/four_meshes.obj";



	// this part acts as a factory for the renderer.
	// Should there be more renderers this one will have its own class.
	auto render_mode = stngs->return_render_mode();
	switch (render_mode)
	{
	case renderMode::STATIC:
		rendererObj = std::make_unique<renderer>(para.get(),stngs.get());
		break;
	case renderMode::ANIMATION:
		rendererObj = std::make_unique<renderer_animation>(para.get(), stngs.get());
		break;
	}
}



renderer_facade::~renderer_facade()
{
}

void renderer_facade::setup()
{
	// building the hittable_list
	world_factory->create();
	// returning the hittable_list pointe from the world_factory to the renderer
	world = world_factory->return_world();

	// getting a reference to the app_settings object
	auto& sett = *stngs;
	// getting the image_settings from the settings object
	settings* img_settings = sett["image"];
	// creating the image object with the camera settings and the parallel object
	auto img = std::make_unique<image>(img_settings, para.get());



	// getting the camera_settings from the settings object
	settings* cam_settings = sett["camera"];
	// building the camera object
	cam = std::make_unique<camera>(cam_settings, std::move(img));

	// getting the writer_settings from the settings object
	settings* wrt_settings = sett["output"];
	// building the writer object
	writer = std::make_unique<output>(wrt_settings, para.get());

	// the renderer setup 
	rendererObj->setup();
}


void renderer_facade::add(std::unique_ptr<hittable>& object)
{
	world->add(std::move(object));
}

void renderer_facade::render()
{
	rendererObj->render(cam.get(), writer.get(), world.get());
}


void renderer_facade::write_file()
{
	// writing the file - it is the job of the writer to just write the file in the rank 0, so no need to check the rank here.
	// also the renderer sets the filename in its render method, so the writer will always write the file with the correct name.
	// also the renderer is in the charge of updating the file contents in its render method, 
	// so the writer just needs to write the file with the current contents.
	writer->write_file();
}



