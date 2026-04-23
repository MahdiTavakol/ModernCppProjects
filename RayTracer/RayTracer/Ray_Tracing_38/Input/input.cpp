#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <mpi.h>

#include "input.h"

input::input(int argc, char** argv,
	std::map<std::string, int> app_set_map,
	communicator* para_,
	std::vector<std::reference_wrapper<std::ostream>> strmVec_):
	para{para_}
{
	init_app_settings();
	initialize(argc, argv);
}

std::unique_ptr<app_settings> input::return_app_settings()
{
	std::move(app_set);
}

void input::initialize(int argc, char** argv)
{
	parse_argv(argv, argc);
	int rank = para->return_rank();
	if (rank == 0)
		logger_function(argc, argv);
}

void input::logger_function(int argc, char** argv)
{
	for (auto& strm_ref : outStreams)
	{
		auto& strm = strm_ref.get();
		strm << "The input: ";
		for (int i = 0; i < argc; i++) {
			if (i % 5 == 0)
				strm << "\n";
			strm << argv[i] << " ";
		}
		strm << std::endl << std::endl;
		camera_settings* cam_settings = settingsObj->return_cam_settings();
		int& image_width = cam_settings->get_image_width();
		int& samples_per_pixel = cam_settings->get_samples_per_pixel();
		int& max_depth = cam_settings->get_max_depth();
		int& vfov = cam_settings->get_vfov();
		double& width_ratio = cam_settings->get_width_ratio();
		double& height_ratio = cam_settings->get_height_ratio();
		int& fps = cam_settings->get_fps();
		int& num_seconds = cam_settings->get_num_seconds();
		strm << "image_width = " << image_width << std::endl;
		strm << "samples_per_pixel = " << samples_per_pixel << std::endl;
		strm << "max_depth = " << max_depth << std::endl;
		strm << "vfov = " << vfov << std::endl;
		strm << "aspect_ratio = " << width_ratio << "/" << height_ratio << std::endl;
		strm << "fps = " << fps << std::endl;
		strm << "num_seconds = " << num_seconds << std::endl;
	}
}


template<typename T>
T input::convert_char(char* _chr)
{
	T _t(0);
	std::string _string(_chr);
	std::stringstream iss(_string);

	if (_chr == nullptr)
	{
		throw std::invalid_argument("Null point passed to char_to_int");
	}

	iss >> _t;

	// Check if the conversion was successful
	if (iss.fail()) {
		throw std::invalid_argument("Conversion failed in char_to_int");
	}

	return _t;
}

void input::init_app_settings()
{
	for (auto& pair : app_set_map)
	{
		if (!strcmp(pair.first.c_str(), "scene"))
			app_set->push_back(std::make_unique<scene_settings>());
		else if (!strcmp(pair.first.c_str(), "camera"))
			app_set->push_back(std::make_unique<camera_settings>());
		else if (!strcmp(pair.first.c_str(), "image"))
			app_set->push_back(std::make_unique<image_settings>());
		else if (!strcmp(pair.first.c_str(), "output"))
			app_set->push_back(std::make_unique<output_settings>());
		else
			throw std::invalid_argument("Nonrecoverable error!");
	}
}

void input::parse_argv(char** argv, int argc)
{
	int iarg = 1;
	while (iarg < argc)
	{
		bool parsed = false;
		// reading the mode 
		{
			int& mode = app_set->return_mode();
			bool original_parsed = parsed;
			parsed = true;
			if (!strcmp(argv[iarg], "-mode") || !strcmp(argv[iarg], "--m"))
			{
				parse_input(argv, argc, iarg, mode);

				// setting everything to the default value for this mode
				app_set->set_mode(mode);
			}
			else
				parsed = false || original_parsed;
		}


		// reading the scene_settings parameters from the command line arguments
		parsed = parse_scene_settings(argv, argc, iarg);
		if (parsed)
			continue;

		//reading the camera_setting parameters from the command line arguments
		parsed = parse_camera_settings(argv, argc, iarg);
		if (parsed)
			continue;

		// reading the image settings
		parsed = parse_image_settings(argv, argc, iarg);
		if (parsed)
			continue;

		// reading the output settings
		parsed = parse_output_settings(argv, argc, iarg);
		if (parsed)
			continue;

		// reading the renderMode
		
		if (!parsed)
		{
			std::cerr << "Unknown command line argument " << argv[iarg] << std::endl;
		}
	}



	// some checks
	settingsObj->check_validity();
}

bool input::parse_scene_settings(char** argv, int argc, int& iarg)
{
	std::string settingName = "world";
	settings* set = app_set->return_settings(settingName);
	if (set == nullptr)
	{
		std::cout << "Warning: no " << settingName << " in the app_settings" << std::endl;
		return false;
	}

	scene_settings* wld_settings = dynamic_cast<scene_settings*>(set);
	if (wld_settings == nullptr)
		throw std::runtime_error("Internal error");

	bool read = true;


	if (!strcmp(argv[iarg], "-mode") || !strcmp(argv[iarg], "--m"))
	{
		int mode;
		parse_input(argv, argc, iarg, mode);
		wld_settings->set_mode(mode);
	}
	else if (!strcmp(argv[iarg], "-obj_file") || !strcmp(argv[iarg], "--obj"))
	{
		std::string obj_file;
		parse_input(argv, argc, iarg, obj_file);
		wld_settings->set_obj_file_name(obj_file);
	}
	else if (!strcmp(argv[iarg], "-mtl_file") || !strcmp(argv[iarg], "--mtl"))
	{
		std::string mtl_file;
		parse_input(argv, argc, iarg, mtl_file);
		wld_settings->set_mtl_file_name(mtl_file);
	}
	else
		return false;
	return true;
}

bool input::parse_camera_settings(char** argv, int argc, int& iarg)
{
	std::string settingName = "camera";
	settings* set = app_set->return_settings(settingName);
	if (set == nullptr)
	{
		std::cout << "Warning: no " << settingName << " in the app_settings" << std::endl;
		return false;
	}

	camera_settings* cam_settings = dynamic_cast<camera_settings*>(set);
	if (cam_settings == nullptr)
		throw std::runtime_error("Internal error");


	if (!strcmp(argv[iarg], "-samples_per_pixel") || !strcmp(argv[iarg], "--s"))
	{
		int& samples_per_pixel = cam_settings->get_samples_per_pixel();
		parse_input(argv, argc, iarg, samples_per_pixel);
	}
	else if (!strcmp(argv[iarg], "-max_depth") || !strcmp(argv[iarg], "--d"))
	{
		int& max_depth = cam_settings->get_max_depth();
		parse_input(argv, argc, iarg, max_depth);
	}
	if (!strcmp(argv[iarg], "-vfov") || !strcmp(argv[iarg], "--v"))
	{
		int& vfov = cam_settings->get_vfov();
		parse_input(argv, argc, iarg, vfov);
	}
	else if (!strcmp(argv[iarg], "-fps") || !strcmp(argv[iarg], "--f"))
	{
		int& fps = cam_settings->get_fps();
		parse_input(argv, argc, iarg, fps);
	}
	else if (!strcmp(argv[iarg], "-num_seconds") || !strcmp(argv[iarg], "--t"))
	{
		int& num_seconds = cam_settings->get_num_seconds();
		parse_input(argv, argc, iarg, num_seconds);
	}
	else
		return false;
	return true;
}

bool input::parse_image_settings(char** argv, int argc, int& iarg)
{
	std::string settingName = "image";
	settings* set = app_set->return_settings(settingName);
	if (set == nullptr)
	{
		std::cout << "Warning: no " << settingName << " in the app_settings" << std::endl;
		return false;
	}

	image_settings* img_settings = dynamic_cast<image_settings*>(set);
	if (img_settings == nullptr)
		throw std::runtime_error("Internal error");



	if (!strcmp(argv[iarg], "-image_width") || !strcmp(argv[iarg], "--w"))
	{
		int& image_width = img_settings->get_image_width();
		parse_input(argv, argc, iarg, image_width);
	}
	else if (!strcmp(argv[iarg], "-aspect_ratio") || !strcmp(argv[iarg], "--a"))
	{
		double& width_ratio = img_settings->get_width_ratio();
		double& height_ratio = img_settings->get_height_ratio();
		parse_input(argv, argc, iarg, width_ratio, height_ratio);
	}
	else
		return false;
	return true;
}



bool input::parse_output_settings(char** argv, int argc, int& iarg) {
	std::string settingName = "output";
	settings* set = app_set->return_settings(settingName);
	if (set == nullptr)
	{
		std::cout << "Warning: no " << settingName << " in the app_settings" << std::endl;
		return false;
	}

	output_settings* out_settings = dynamic_cast<output_settings*>(set);
	if (out_settings == nullptr)
		throw std::runtime_error("Internal error");


	if (!strcmp(argv[iarg], "-output_type") || !strcmp(argv[iarg], "--o"))
	{
		outputType outType = out_settings->return_type();
		std::string output_type_str(argv[iarg + 1]);
		if (output_type_str == "SERIAL")
		{
			outType = outputType::SERIAL;
		}
		else if (output_type_str == "PARALLEL")
		{
			outType = outputType::PARALLEL;
		}
		else
		{
			std::cerr << "Invalid output mode: " << output_type_str << std::endl;
		}
		iarg += 2;
	}
	else if (!strcmp(argv[iarg], "-output_mode") || !strcmp(argv[iarg], "--om"))
	{
		outputMode outMode = out_settings->return_outputMode();
		std::string output_mode_str(argv[iarg + 1]);
		if (output_mode_str == "P3")
		{
			outMode = outputMode::P3;
		}
		else if (output_mode_str == "P6")
		{
			outMode = outputMode::P6;
		}
		else
		{
			std::cerr << "Invalid output mode: " << output_mode_str << std::endl;
		}
		iarg += 2;
	}
	else if (!strcmp(argv[iarg], "-output_file") || !strcmp(argv[iarg], "--of"))
	{
		std::string output_file_name(argv[iarg + 1]);
		out_settings->set_file_name(output_file_name);
		iarg += 2;
	}
	else
		return false;
	return true;
}


bool input::parse_render_mode(char** argv, int argc, int& iarg)
{
	renderMode& render_mode = settingsObj->return_render_mode();
	bool original_parsed = parsed;
	parsed = true;
	if (!strcmp(argv[iarg], "-render_mode") || !strcmp(argv[iarg], "--r"))
	{
		std::string render_mode_str(argv[iarg + 1]);
		if (render_mode_str == "STATIC")
		{
			render_mode = renderMode::STATIC;
		}
		else if (render_mode_str == "ANIMATION")
		{
			render_mode = renderMode::ANIMATION;
		}
		iarg += 2;
	}
	else
		parsed = false || parsed;
}
