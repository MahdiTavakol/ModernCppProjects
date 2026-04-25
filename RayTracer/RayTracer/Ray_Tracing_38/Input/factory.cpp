#include "factory.hpp"
#include "../Renderer/Renderer.h"
#include "../Algorithms/mpiComm.h"
#include "../Output/output_serial.h"
#include "../Output/output_parallel.h"

factory::factory(int argc, char** argv, int mode_,
	MPI_Comm comm_):
	mode{ mode_ }
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


	// parsing the input arguments for the parallel communicator from the cml
	std::unique_ptr<settings> comm_settings = std::make_unique<communicator_settings>();
	input::set_communicator_settings(argc, argv, comm_settings.get());
	// the parallel class in charge of the parallel communicator
	para = std::make_unique<mpiComm>(comm_, comm_settings.get());


	// changing the settings based on the user input
	in = std::make_unique<input>(argc, argv, mode_,app_set_map, para.get());
	// getting the app_settings object from the in
	stngs = in->return_app_settings();

	// creating the dedicated factories

	// reference to the settings
	auto& sett = *stngs;


	// getting the settings for the renderer_factory object
	settings* renderer_settings = sett["renderer"];
	// the renderer factory
	rend_factory = std::make_unique<renderer_factory>(renderer_settings,para.get());


	// getting the settings for the scene_factory object
	settings* scene_settings = sett["scene"];
	// the world_factory is in charge of lazy creation of the scene.
	world_factory = std::make_unique<scene_factory>(scene_settings, para.get());



}

void factory::create()
{
	// getting a reference to the app_settings object
	auto& sett = *stngs;

	// objects without dedicated factories
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
	// getting the output writer type
	// not an ideal situation since it couples the factory to the output_settings
	output_settings* wrt_sett = dynamic_cast<output_settings*>(wrt_settings);
	outputType wrt_type = wrt_sett->return_type();
	// building the writer object
	switch (wrt_type)
	{
	case outputType::SERIAL:
		writer = std::make_unique<output_serial>(wrt_settings, para.get());
		break;
	case outputType::PARALLEL:
		writer = std::make_unique<output_parallel>(wrt_settings, para.get());
		break;
	default:
		throw std::invalid_argument("Unknown output mode");
	}

	// objects with dedicated factories
	// building the renderer object
	rend_factory->create();
	// building the hittable_list
	world_factory->create();
}

std::unique_ptr<communicator> factory::return_comm()
{
	if (para == nullptr)
		throw std::runtime_error("This object has already been returned!");
	return std::move(para);
}

std::unique_ptr<camera> factory::return_camera()
{
	if (cam == nullptr)
		throw std::runtime_error("This object has already been returned!");
	return std::move(cam);
}

std::unique_ptr<output> factory::return_writer()
{
	if (writer == nullptr)
		throw std::runtime_error("This object has already been returned!");
	return std::move(writer);
}

std::unique_ptr<hittable_list> factory::return_world()
{
	// returning the hittable_list pointe from the world_factory
	auto world = world_factory->return_object();
	if (world == nullptr)
		throw std::runtime_error("This object has already been returned!");
	return std::move(world);
}

std::unique_ptr<renderer> factory::return_renderer()
{
	// returning the renderer object from its factory
	auto rend = rend_factory->return_object();
	if (rend == nullptr)
		throw std::runtime_error("This object has already been returned!");
	return std::move(rend);
}