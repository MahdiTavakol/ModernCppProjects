#include <string>
#include <cstring>
#include <sstream>
#include <iostream>

#include "input.h"

#include "image_settings.h"
#include "output_settings.h"
#include "scene_settings.h"
#include "camera_settings.h"
#include "path_settings.h"

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
	if (argc <= 1)
		throw std::runtime_error("Not enough cmd arguments");
	else if (strcmp(argv[1], "file"))
	{
		std::string file_name = argv[2];
		std::unique_ptr<std::fstream> file_stream = std::make_unique<std::fstream>(file_name, std::ios::in);

		if (!file_stream->is_open())
			throw std::invalid_argument("File could not be found!");
		input_stream = std::move(file_stream);
	}
	else
	{
		this->fill_iostream(argc, argv);
	}

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
	// initializing the app_settings object
	for (auto& pair : app_set_map)
	{
		if (!pair.first.compare("scene"))
			app_set->push_back(std::make_unique<scene_settings>());
		else if (!pair.first.compare("camera"))
			app_set->push_back(std::make_unique<camera_settings>());
		else if (!pair.first.compare("image"))
			app_set->push_back(std::make_unique<image_settings>());
		else if (!pair.first.compare("output"))
			app_set->push_back(std::make_unique<output_settings>());
		else if (!pair.first.compare("path"))
			app_set->push_back(std::make_unique<path_settings>());
		else
			throw std::invalid_argument("Nonrecoverable error!");
	}
}

void input::parse_file()
{
	std::string line;

	// reading commands and adding to them to the app_settings
	while (getline(*input_stream, line))
	{
		std::stringstream ss(line);
		std::string text;
		ss >> text;

		if (!text.compare("-render_mode"))
		{
			parse_render_mode(ss);
			continue;
		}

		std::string newCmd;
		getline(ss, newCmd);
		if (!app_set->add_cmd(text, newCmd) && text.compare("rank_config"))
			throw std::invalid_argument("Wrong input argument!");
	}

	// parsing the commands
	app_set->parse_commands();
	// checking the validity of parameters
	app_set->check_validity();

}

void input::parse_render_mode(std::stringstream& ss)
{
	renderMode render_mode;
	std::string render_mode_str;
	ss >> render_mode_str;

	if (render_mode_str == "STATIC")
	{
		render_mode = renderMode::STATIC;
	}
	else if (render_mode_str == "ANIMATION")
	{
		render_mode = renderMode::ANIMATION;
	}
	else
	{
		throw std::invalid_argument("Wrong render mode keyword!");
	}
	app_set->set_render_mode(render_mode);
}

void input::fill_iostream(int argc, char** argv)
{
	input_stream = std::make_unique<std::stringstream>();

	for (int i = 1; i < argc; i++)
	{
		std::string text(argv[i]);
		*input_stream >> text;
	}

}

void input::set_communicator_settings(int argc, char** argv, settings* com_settings)
{
	int iarg = 1;
	std::string newCmd = "size_config";
	while (iarg < argc)
	{
		if (!strcmp(argv[iarg], newCmd.c_str()))
		{
			if (iarg + 2 >= argc)
				throw std::invalid_argument("Corrupted input!");
			std::string text1(argv[iarg + 1]);
			std::string text2(argv[iarg + 2]);
			newCmd += " " + text1 + " " + text2;
			com_settings->add_cmd(newCmd);
			break;
		}
	}
	com_settings->parse_commands();
}

void input::check_compatibility()
{
	// just animation mode has fps and num_seconds
	auto renderMode = app_set->return_render_mode();
	settings* pth_settings =  (*app_set)["path"];
	path_settings* sett = dynamic_cast<path_settings*>(pth_settings);
	int num_seconds;
	int fps;
	sett->return_movie_params(num_seconds, fps);
	if (renderMode == renderMode::STATIC)
		if (fps || num_seconds)
			std::cout << "Warning the path variable will be ignored for the static rendering!" << std::endl;




	
}
