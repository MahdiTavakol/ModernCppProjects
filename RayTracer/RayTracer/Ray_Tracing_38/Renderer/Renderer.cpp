#include "Renderer.h"


renderer::renderer(int argc, char** argv, int _mode, 
				   std::string _filename, 
	               std::array<int, 2> size_config_,
	               MPI_Comm comm_)
	: mode(_mode), filename(_filename)
{
	para = std::make_unique<mpiParallel>(comm_,size_config_);
	in = std::make_unique<input>(argc, argv, mode);
	world_factory = std::make_unique<scene_factory>(mode,para);
	if (!filename.empty())
		writer = std::make_unique<class write>(filename);
}

renderer::renderer(int argc, char** argv, int _mode, std::string _filename,
	std::string obj_file_, std::array<int, 2> size_config_, MPI_Comm comm_):
	renderer{argc,argv,_mode,_filename, size_config_,comm_}
{
	obj_file_name = obj_file_;
	world_factory = std::make_unique<scene_factory>(mode, para, obj_file_name);
}


renderer::~renderer()
{}

void renderer::setup()
{
	// building the hittable_list
	world_factory->create();
 	world = world_factory->return_world();


	// building the camera object
	cam = std::make_unique<camera_parallel>(in,para);


	// the default camera location
	//camera_location = point3(26, 3, 6);


	// cases with extra setups
	switch (mode)
	{
		case CORNELL_BOX:
			camera_location = point3(278, 278, -800);
			break;
		case OBJ_MODEL_PARALLEL:
			cam = std::make_unique<camera_derived>(in);
			break;
	}

	//world = std::make_unique<hittable_list>();


	//cam->move_camera(camera_location);
}


void renderer::add(std::unique_ptr<hittable>& object)
{
	world->add(std::move(object));
}

void renderer::render()
{
	cam->render(*world);
}


void renderer::write_file()
{
	int image_width, image_height;
	update_c_array();
	cam->return_image_size(image_width, image_height);

	if (writer == nullptr)
		writer = std::make_unique<class write>("test.ppm");
	writer->reset(c_array, image_width, image_height);

	int rank = para->return_rank();
	if (rank == 0) writer->write_file();
}

void renderer::update_c_array()
{
	c_array = cam->return_color_array_ptr();
}



