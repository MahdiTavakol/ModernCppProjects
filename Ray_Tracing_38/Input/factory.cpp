#include "factory.hpp"
#include "../Renderer/Renderer.h"
#include "../Algorithms/mpiComm.h"
#include "../Output/output_serial.h"
#include "../Output/output_parallel.h"
#include "../Algorithms/simpleComm.h"
#include "../Algorithms/image.h"
#include "../Algorithms/image_async.h"
#include "../Output/output_async.h"
#include "renderer_settings.h"

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

	// parsing the logger arguments for the Logger class from the cml
	//std::unique_ptr<settings> error_settings = std::make_unique<logger_settings>();
	//input::set_logger_settings(argc, argv, error_settings.get());
	// the Logger class is in charge of logging output
	errorResource = std::make_unique<Logger>();
	error = errorResource.get();


	/* Since the profiler is dependent on the communicator and the logger objects,
	* I put it here after those objects are created. 
	* However, it needed to be created ASAP to be a more accurate measure of the program
	* running time.
	*/
	timerResource = std::make_unique<profiler>(para.get(), error);
	timer = timerResource.get();
	// changing the settings based on the user input
	in = std::make_unique<input>(argc, argv, mode_,app_set_map, para.get());
	// parsing the cmd args or the input file
	in->parse_file();
	// getting the app_settings object from the in
	stngs = in->return_app_settings();


	// checking the profiler settings
	settings* prof_settings = (*stngs)["profiler"];;
	// the profiler object
	profiler_settings* prof_conv = dynamic_cast<profiler_settings*>(prof_settings);
	if (prof_conv == nullptr)
		throw std::runtime_error("Unrecoverable error!");
	int nProfiling;
	prof_conv->return_profiling_info(profiling, nProfiling);
	timers.reserve(nProfiling);
	for (int i = 0; i < nProfiling; i++)
	{
		timers.push_back(std::make_unique<profiler>(para.get(), error));
	}
	render_profiler = renderer_factory::create_profiler(para.get(), error, timer);
	//do we do profiling
	if (nProfiling > 1)
		error->reset_mode(print_mode::PROFILING);

	// getting the renderer setting to check if we need to shut down the logger or not
	settings* rend_settings = (*stngs)["renderer"];
	renderer_settings* rend_conv = dynamic_cast<renderer_settings*>(rend_settings);
	if (rend_conv == nullptr)
		error->print_error("Unrecoverable error!");
	int nprofiling;
	rend_conv->return_nprofiling(nprofiling);
	if (nprofiling > 1) {
		// supressing all the printings in the logger function
		error->reset_mode(print_mode::PROFILING);
	}


	// getting the settings for the scene_factory object
	settings* scene_settings = (*stngs)["scene"];
	// the world_factory is in charge of lazy creation of the scene.
	world_factory = std::make_unique<scene_factory>(scene_settings,error, para.get(), timer);
	// since settings in some classes are dependent on hittable_list
	// we first create that.
	// building the hittable_list
	world_factory->create();
	world = world_factory->return_object();

	// setting scene dependent settings
	stngs->set_from_scene(*world);


	// creating the dedicated factories

	// reference to the settings
	auto& sett = *stngs;

	// getting the settings for the renderer_factory object
	settings* renderer_settings = sett["renderer"];
	// the renderer factory
	rend_factory = std::make_unique<renderer_factory>(
		renderer_settings,
		para.get(),
		error,
		timer);

}

factory::factory(std::vector<std::string> argv_vec, int mode_,
	MPI_Comm comm_,
	Logger* error_, profiler* timer_) :
	mode{ mode_ },
	timer{timer_},
	error{ error_}
{
	int argc = argv_vec.size();
	char** argv;
	argv = new char* [argc];
	for (int i = 0; i < argc; i++)
	{
		int size = static_cast<int>(argv_vec[i].size());
		argv[i] = new char[size + 1];
		for (int j = 0; j < size; j++)
			argv[i][j] = argv_vec[i][j];
		argv[i][size] = '\0';
	}

	std::unique_ptr<settings> comm_settings = std::make_unique<communicator_settings>();
	input::set_communicator_settings(argc, argv, comm_settings.get());
	para = std::make_unique<mpiComm>(comm_, comm_settings.get());
	auto dummy_timer = std::make_unique<profiler>(para.get(), error);
	in = std::make_unique<input>(argc, argv, mode_, app_set_map, para.get());
	in->parse_file();
	stngs = in->return_app_settings();
	settings* prof_settings = (*stngs)["profiler"];;
	profiler_settings* prof_conv = dynamic_cast<profiler_settings*>(prof_settings);
	if (prof_conv == nullptr)
		throw std::runtime_error("Unrecoverable error!");
	int nProfiling;
	prof_conv->return_profiling_info(profiling, nProfiling);
	timers.reserve(nProfiling);
	for (int i = 0; i < nProfiling; i++)
	{
		timers.push_back(std::make_unique<profiler>(para.get(), error));
	}
	if (nProfiling > 1)
		error->reset_mode(print_mode::PROFILING);

	settings* rend_settings = (*stngs)["renderer"];
	renderer_settings* rend_conv = dynamic_cast<renderer_settings*>(rend_settings);
	if (rend_conv == nullptr)
		error->print_error("Unrecoverable error!");
	int nprofiling;
	rend_conv->return_nprofiling(nprofiling);
	if (nprofiling > 1) {
		// supressing all the printings in the logger function
		error->reset_mode(print_mode::PROFILING);
	}
	settings* scene_settings = (*stngs)["scene"];
	world_factory = std::make_unique<scene_factory>(scene_settings, error, para.get(), timer);

	world_factory->create();
	world = world_factory->return_object();


	stngs->set_from_scene(*world);

	auto& sett = *stngs;

	// getting the settings for the renderer_factory object
	settings* renderer_settings = sett["renderer"];
	// the renderer factory
	rend_factory = std::make_unique<renderer_factory>(
		renderer_settings,
		para.get(),
		error,
		timer);


	for (int i = 0; i < argc; i++)
	{
		delete[] argv[i];
	}
	delete[] argv;

}

void factory::create()
{
	// getting a reference to the app_settings object
	auto& sett = *stngs;
	

	// objects without dedicated factories

	// getting the writer_settings from the settings object
	settings* wrt_settings = sett["output"];
	// getting the output writer type
	// not an ideal situation since it couples the factory to the output_settings
	output_settings* wrt_sett = dynamic_cast<output_settings*>(wrt_settings);
	outputType wrt_type = wrt_sett->return_type();


	// creating the image
	create_image();



	// getting the camera_settings from the settings object
	settings* cam_settings = sett["camera"];
	// building the camera object
	cam = std::make_unique<camera>(cam_settings,img.get());




	// building the writer object
	switch (wrt_type)
	{
	case outputType::SERIAL:
		writer = std::make_unique<output_serial>(wrt_settings, para.get());
		break;
	case outputType::PARALLEL:
		writer = std::make_unique<output_parallel>(wrt_settings, para.get());
		break;
	case outputType::ASYNC:
		writer = std::make_unique<output_async>(wrt_settings, para.get());
		break;
	default:
		throw std::invalid_argument("Unknown output mode");
	}

	// objects with dedicated factories
	// building the renderer object
	rend_factory->create();

}


void factory::create_image()
{
	// getting a reference to the app_settings object
	auto& sett = *stngs;


	// getting the image_settings from the settings object
	settings* img_settings = sett["image"];

	// getting the writer_settings from the settings object
	settings* wrt_settings = sett["output"];
	// getting the output writer type
	// not an ideal situation since it couples the factory to the output_settings
	output_settings* wrt_sett = dynamic_cast<output_settings*>(wrt_settings);
	outputType wrt_type = wrt_sett->return_type();


	switch (wrt_type)
	{
	case outputType::SERIAL:
	case outputType::PARALLEL:
		img = std::make_unique<image>(img_settings, para.get());
		break;
	case outputType::ASYNC:
		img = std::make_unique<image_async>(img_settings, para.get());
		break;
	default:
		throw std::invalid_argument("Unknown output mode");
	}
}

// since we do not know how many images
// will be requested we generate images on the fly
// whenever the return_image method is called
std::unique_ptr<image> factory::return_image()
{
	if (img == nullptr)
	{
		std::cout << "The image has already been returned, recreating it!";
		create_image();
	}
	return std::move(img);
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
	if (world == nullptr)
		throw std::runtime_error("This object has already been returned!");
	return std::move(world);
}

std::unique_ptr<material_list> factory::return_mtl_list()
{
	// returning the hittable_list pointe from the world_factory
	auto list = world_factory->return_mtl_list();
	if (list == nullptr)
		throw std::runtime_error("This object has already been returned!");
	return std::move(list);
}

std::unique_ptr<renderer> factory::return_renderer()
{
	// returning the renderer object from its factory
	auto rend = rend_factory->return_object();
	if (rend == nullptr)
		throw std::runtime_error("This object has already been returned!");
	return std::move(rend);
}

std::unique_ptr<Logger> factory::return_error()
{
	// returning the error
	if (errorResource == nullptr)
		throw std::runtime_error("The logger has already returned.. This program does not support multiple logger");
	return std::move(errorResource);
}

std::unique_ptr<profiler> factory::return_timer()
{
	if (timerResource == nullptr)
		throw std::runtime_error("The timer has already returned.. This program does not support multiple timers");
	return std::move(timerResource);
}


void factory::return_profiling_info(
	bool& profiling_,
	std::unique_ptr<renderer>& render_profiler_,
	std::vector<std::unique_ptr<profiler>>& timers_)
{
	profiling_ = profiling;
	render_profiler_ = std::move(render_profiler);
	timers_ = std::move(timers);
}