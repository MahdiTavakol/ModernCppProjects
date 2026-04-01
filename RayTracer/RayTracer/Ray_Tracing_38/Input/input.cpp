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


	this->defocus_angle = 0;
	this->focus_dist = 10.0;

	this->background = color(0.7, 0.8, 1.00);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) input_logger = true;
	if (input_logger) logger_function(argc, argv);
}

input::input(int argc, char** argv, camera_settings* cam_settings_):
	cam_settings{cam_settings_}
{
	this->image_width = cam_settings->get_image_width();
	this->samples_per_pixel = cam_settings->get_samples_per_pixel();
	this->max_depth = cam_settings->get_max_depth();
	this->vfov = cam_settings->get_vfov();
	this->width_ratio = cam_settings->get_width_ratio();
	this->height_ratio = cam_settings->get_height_ratio();
	this->defocus_angle = cam_settings->get_defocus_angle();
	this->focus_dist = cam_settings->get_focus_dist();
	this->lookfrom = cam_settings->get_lookfrom();
	this->lookat = cam_settings->get_lookat();
	this->vup = cam_settings->get_vup();
	this->background = cam_settings->get_background();
	this->fps = cam_settings->get_fps();
	this->num_seconds = cam_settings->get_num_seconds();

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

const int& input::get_image_width() const
{
	return this->image_width;
}
const int& input::get_samples_per_pixel() const
{
	return this->samples_per_pixel;
}
const int& input::get_max_depth() const
{
	return this->max_depth;
}
const int& input::get_vfov() const
{
	return this->vfov;
}
const double& input::get_width_ratio() const
{
	return width_ratio;
}
const double& input::get_height_ratio() const
{
	return height_ratio;
}
const double& input::get_defocus_angle() const
{
	return defocus_angle;
}
const double& input::get_focus_dist() const
{
	return focus_dist;
}
const point3& input::get_lookfrom() const
{
	return lookfrom;
}
const point3& input::get_lookat() const
{
	return lookat;
}
const point3& input::get_vup() const
{
	return lookat;
}
const color& input::get_background() const
{
	return background;
}
const int& input::get_fps() const
{
	return fps;
}
const int& input::get_num_seconds() const
{
	return num_seconds;
}

void input::fps_num_seconds(int& _fps, int& _num_seconds) const
{
	_fps = this->fps;
	_num_seconds = this->num_seconds;
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
			double& with_ratio = cam_settings->get_width_ratio();
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
