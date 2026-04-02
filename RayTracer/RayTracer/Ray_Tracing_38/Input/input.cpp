#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <mpi.h>

#include "input.h"


input::input(int argc, char** argv, int _image_width, int _samples_per_pixel,
	int _max_depth, int _vfov, double _width_ratio, double _height_ratio,
	camera_settings* cam_settings_)
	: cam_settings{ cam_settings_ }, input_logger{ false }

{
	int& image_width = cam_settings->get_image_width();
	int& samples_per_pixel = cam_settings->get_samples_per_pixel();
	int& max_depth = cam_settings->get_max_depth();
	int& vfov = cam_settings->get_vfov();
	double& width_ratio = cam_settings->get_width_ratio();
	double& height_ratio = cam_settings->get_height_ratio();
	int& fps = cam_settings->get_fps();
	int& num_seconds = cam_settings->get_num_seconds();

	image_width = _image_width;
	samples_per_pixel = _samples_per_pixel;
	max_depth = _max_depth;
	vfov = _vfov;
	width_ratio = _width_ratio;
	height_ratio = _height_ratio;
	fps = 60;
	num_seconds = 10;


	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) input_logger = true;
	if (input_logger) logger_function(argc, argv);
}

input::input(int argc, char** argv,
	camera_settings* cam_settings_,
	std::vector<std::reference_wrapper<std::ostream>> strmVec_):
	cam_settings{cam_settings_}
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) input_logger = true;
	if (input_logger) logger_function(argc, argv);
}

void input::logger_function(int argc, char** argv)
{
 	logfile.open("RayTracingInput.log", std::ios::out);
	if (!logfile.is_open())
		std::cerr << "Cannot open the RayTracing.log file for logging" << std::endl;

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


void input::parse_argv(char** argv, int argc)
{
	int iarg = 1;
	while (iarg < argc)
	{
		if (!strcmp(argv[iarg], "-image_width") || !strcmp(argv[iarg], "--w"))
		{
			int& image_width = cam_settings->get_image_width();
			parse_input(argv, argc, iarg, image_width);
		}
		if (!strcmp(argv[iarg], "-samples_per_pixel") || !strcmp(argv[iarg], "--s"))
		{
			int& samples_per_pixel = cam_settings->get_samples_per_pixel();
			parse_input(argv, argc, iarg, samples_per_pixel);
		}
		if (!strcmp(argv[iarg], "-max_depth") || !strcmp(argv[iarg], "--d"))
		{
			int& max_depth = cam_settings->get_max_depth();
			parse_input(argv, argc, iarg, max_depth);
		}
		if (!strcmp(argv[iarg], "-vfov") || !strcmp(argv[iarg], "--v"))
		{
			int& vfov = cam_settings->get_vfov();
			parse_input(argv, argc, iarg, vfov);
		}
		if (!strcmp(argv[iarg], "-aspect_ratio") || !strcmp(argv[iarg], "--a"))
		{
			double& width_ratio = cam_settings->get_width_ratio();
			double& height_ratio = cam_settings->get_height_ratio();
			parse_input(argv, argc, iarg, width_ratio, height_ratio);
		}
		if (!strcmp(argv[iarg], "-fps") || !strcmp(argv[iarg], "--f"))
		{
			int& fps = cam_settings->get_fps();
			parse_input(argv, argc, iarg, fps);
		}
		if (!strcmp(argv[iarg], "-num_seconds") || !strcmp(argv[iarg], "--t"))
		{
			int& num_seconds = cam_settings->get_num_seconds();
			parse_input(argv, argc, iarg, num_seconds);
		}
		else
		{
			std::cerr << "Unknown command line argument " << argv[iarg] << std::endl;
		}
	}
}
