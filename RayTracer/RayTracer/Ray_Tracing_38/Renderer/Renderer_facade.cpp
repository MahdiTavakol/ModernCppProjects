#include "Renderer_facade.h"


renderer_facade::renderer_facade(int argc, char** argv, int _mode,
	std::string _filename,
	std::array<int, 2> size_config_,
	MPI_Comm comm_)
	:
	mode{ _mode },
	filename{ _filename }
{
	/* cam_setting sets the default parameters for the modes,
	 * then the input class can overwrite those parameters based on the user input
	 */
	cam_settings = std::make_unique<camera_settings>(mode);
	// the parallel class in charge of the parallel communicator
	para = std::make_unique<mpiComm>(comm_, size_config_);
	// changing the camera settings based on the user input
	in = std::make_unique<input>(argc, argv, cam_settings, para);
	// the world_factory is in charge of lazy creation of the scene.
	world_factory = std::make_unique<scene_factory>(mode, para);

	// think about how the run mode (animation vs static) 
	// and the size should be parsed from the argv, argc
	// May be argc, argv goes to a list of strings and
	// each parser takes its own args from the list and removes its from the list
	std::array<int, 2> size_config{ 4, 2 };
	std::string obj_file = "../models/Simple/four_meshes.obj";
	std::unique_ptr<renderer> rendererObj = std::make_unique<renderer>(argc, argv, mode, filename, obj_file, size_config);
	//std::unique_ptr<renderer> render = std::make_unique<renderer_animation>(argc,argv,mode,filename);

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

	// creating the image object with the camera settings and the parallel object
	auto img = std::make_unique<image>(cam_settings, para);

	// building the camera object
	cam = std::make_unique<camera>(cam_settings, std::move(img));

	// the renderer setup 
	rendererObj->setup();
}


void renderer_facade::add(std::unique_ptr<hittable>& object)
{
	world->add(std::move(object));
}

void renderer_facade::render()
{
	cam->render(*world);
}


void renderer_facade::write_file()
{
	// transfering the ownership of the image from the camera to the writer
	std::unique_ptr<image> img = cam->return_image();
	// creating the writer object with the filename, the image and the parallel object
	writer = std::make_unique<output_serial>(filename, std::move(img), para);
	// writing the file - it is the job of the writer to just write the file in the rank 0, so no need to check the rank here.
	writer->write_file();
}



