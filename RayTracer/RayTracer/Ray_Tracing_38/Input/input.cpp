#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <mpi.h>

#include "input.h"


input::input(int argc, char** argv, int mode, std::vector<std::reference_wrapper<std::ostream>> strmVec_):
	image_width(150), samples_per_pixel(150), max_depth(50),
	vfov(20), 
	width_ratio(16.0), height_ratio(9.0), 
	fps(1), num_seconds(1), input_logger(false),
	outStreams{strmVec_}
{
	default_params(mode);
	parse_argv(argv, argc);


	int rank;
	//MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//if (rank == 0) 
	input_logger = true;
	if (input_logger) logger_function(argc, argv);

}


input::input(int argc, char** argv, int _image_width, int _samples_per_pixel,
	int _max_depth, int _vfov, double _width_ratio, double _height_ratio)
	: image_width(_image_width), samples_per_pixel(_samples_per_pixel), max_depth(_max_depth),
	vfov(_vfov), width_ratio(_width_ratio), height_ratio(_height_ratio),
	fps(60), num_seconds(10), input_logger(false)

{
	this->lookfrom = point3(13, 2, 3);
	this->lookat = point3(0, 0, 0);
	this->vup = point3(0, 1, 0);


	this->defocus_angle = 0.6;
	this->defocus_angle = 0;
	this->focus_dist = 10.0;

	this->background = color(0.7, 0.8, 1.00);

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
		strm << "image_width = " << image_width << std::endl;
		strm << "samples_per_pixel = " << samples_per_pixel << std::endl;
		strm << "max_depth = " << max_depth << std::endl;
		strm << "vfov = " << vfov << std::endl;
		strm << "aspect_ratio = " << width_ratio << "/" << height_ratio << std::endl;
		strm << "fps = " << fps << std::endl;
		strm << "num_seconds = " << num_seconds << std::endl;
	}
}

void input::setup_camera(camera* cam) const
{
	cam->aspect_ratio = this->width_ratio / this->height_ratio;
	cam->image_width = this->image_width;
	cam->samples_per_pixel = this->samples_per_pixel;
	cam->max_depth = this->max_depth;
	//cam.vfov = 90;
	cam->vfov = this->vfov;
	cam->lookfrom = this->lookfrom;
	cam->lookat = this->lookat;
	cam->vup = this->vup;
	cam->defocus_angle = this->defocus_angle;
	cam->focus_dist = this->focus_dist;
	cam->background = this->background;

	cam->initialize();
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


void input::default_params(const int mode_)
{
	this->lookfrom = point3(13, 2, 3);
	this->lookat = point3(0, 0, 0);
	this->vup = point3(0, 1, 0);

	lookfrom = vec3(16200, 15180, 7690);
	lookat = vec3(-800, 180, -310);

	lookfrom = point3(3, 3, 5);
	lookat = point3(0, 0, 0);
	vup = vec3(0, 1, 0);
	vfov = 45;


	this->defocus_angle = 0.6;
	this->defocus_angle = 0;
	this->focus_dist = 10.0;
	this->background = color(0, 0, 0);


	if (mode_ == QUADS)
	{
		this->image_width = 1080;
		this->samples_per_pixel = 100;
		this->max_depth = 50;

		this->width_ratio = 1.0;
		this->height_ratio = 1.0;

		this->vfov = 80;
		this->lookfrom = point3(0, 0, 9);
		this->lookat = point3(0, 0, 0);
		this->vup = vec3(0, 1, 0);

		this->defocus_angle = 0;
	}
	else if (mode_ == TWO_LIGHTS || mode_ == SIMPLE_LIGHT)
	{
		this->lookfrom = point3(26, 3, 6);
		this->lookat = point3(0, 1, 0);
		this->defocus_angle = 0;
		this->vfov = 20;
		this->background = color(0, 0, 0);
	}
	else if (mode_ == CORNELL_BOX || mode_ == TWO_BOXES || mode_ == TWO_BOXES_ROTATED)
	{
		this->lookfrom = point3(278, 278, -800);
		this->lookat = point3(278, 278, 0);
		this->vfov = 40;
		this->vup = vec3(0, 1, 0);
		this->defocus_angle = 0;
		this->background = color(0, 0, 0);
		this->width_ratio = 1.0;
		this->height_ratio = 1.0;
	}
	else if (mode_ == CORNELL_SMOKE)
	{
		this->width_ratio = 1.0;
		this->height_ratio = 1.0;
		this->background = color(0, 0, 0);

		this->vfov = 40;
		this->lookfrom = point3(278, 278, -800);
		this->lookat = point3(278, 278, 0);
		this->vup = vec3(0, 1, 0);
		this->defocus_angle = 0;
	}
	else if (mode_ == RANDOM_SPHERES_ANIMATED)
	{
		this->fps = 60;
		this->num_seconds = 10;
		this->background = color(0.7, 0.8, 1.00);
		// The rest of the thing
	}
	else if (mode_ == SIMPLE_2D_PARALEL_TEST)
	{
		this->lookfrom = point3(0, 0, 100);
		this->lookat = point3(0, 0, 0);
		this->vup = vec3(0, 1, 0);
		this->vfov = 20;
		this->defocus_angle = 0;
	}
	else this->background = color(0.7, 0.8, 1.00);
}

void input::parse_argv(char** argv, int argc)
{
	int iarg = 1;
	while (iarg < argc)
	{
		if (!strcmp(argv[iarg], "-image_width") || !strcmp(argv[iarg], "--w"))
		{
			parse_input(argv, argc, iarg, image_width);
		}
		if (!strcmp(argv[iarg], "-samples_per_pixel") || !strcmp(argv[iarg], "--s"))
		{
			parse_input(argv, argc, iarg, samples_per_pixel);
		}
		if (!strcmp(argv[iarg], "-max_depth") || !strcmp(argv[iarg], "--d"))
		{
			parse_input(argv, argc, iarg, max_depth);
		}
		if (!strcmp(argv[iarg], "-vfov") || !strcmp(argv[iarg], "--v"))
		{
			parse_input(argv, argc, iarg, vfov);
		}
		if (!strcmp(argv[iarg], "-aspect_ratio") || !strcmp(argv[iarg], "--a"))
		{
			parse_input(argv, argc, iarg, width_ratio, height_ratio);
		}
		if (!strcmp(argv[iarg], "-fps") || !strcmp(argv[iarg], "--f"))
		{
			parse_input(argv, argc, iarg, fps);
		}
		if (!strcmp(argv[iarg], "-num_seconds") || !strcmp(argv[iarg], "--t"))
		{
			parse_input(argv, argc, iarg, num_seconds);
		}
		else
		{
			std::cerr << "Unknown command line argument " << argv[iarg] << std::endl;
		}
	}
}
