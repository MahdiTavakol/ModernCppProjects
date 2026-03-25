#ifndef INPUT_H
#define INPUT_H

#include <fstream>
#include <map>
#include <variant>
#include "../Algorithms/camera.h"


class input
{
public:
	input(int argc, char** argv, int mode,
		std::vector<std::reference_wrapper<std::ostream>> strmVec_ =
		{ 
			std::vector<std::reference_wrapper<std::ostream>>{
				std::ref(std::cout)
			} 
		});
	input(int argc, char** argv, int _image_width, int _samples_per_pixel,
		int _max_depth, int _vfov, double _width_ratio, double _height_ratio);

	const int& get_image_width() const
	{
		return this->image_width;
	}
	const int& get_samples_per_pixel() const
	{
		return this->samples_per_pixel;
	}
	const int& get_max_depth() const
	{
		return this->max_depth;
	}
	const int& get_vfov() const
	{
		return this->vfov;
	}
	const double& get_width_ratio() const
	{
		return width_ratio;
	}
	const double& get_height_ratio() const
	{
		return height_ratio;
	}
	const double& get_defocus_angle() const
	{
		return defocus_angle;
	}
	const double& get_focus_dist() const
	{
		return focus_dist;
	}
	const point3& get_lookfrom() const
	{
		return lookfrom;
	}
	const point3& get_lookat() const
	{
		return lookat;
	}
	const point3& get_vup() const
	{
		return lookat;
	}
	const color& get_background() const
	{
		return background;
	}
	const int& get_fps() const
	{
		return fps;
	}
	const int& get_num_seconds() const
	{
		return num_seconds;
	}
	void fps_num_seconds(int& _fps, int& _num_seconds) const
	{
		_fps = this->fps;
		_num_seconds = this->num_seconds;
	}
	void setup_camera(camera* cam) const;

protected:
	template<typename T>
	T convert_char(char* _chr);
	int image_width;
	int samples_per_pixel;
	int max_depth;
	int vfov;
	double width_ratio;
	double height_ratio;
	double defocus_angle;
	double focus_dist;

	point3 lookfrom;
	point3 lookat;
	point3 vup;

	color background;

	int fps;
	int num_seconds;

	bool input_logger;
	std::ofstream logfile;
	std::vector<std::reference_wrapper<std::ostream>> outStreams;
	void logger_function(int argc, char** argv);

	// default parameters
	void default_params(const int mode_);
	// parsing argv
	void parse_argv(char** argv, int argc);
	// parsing parameters
	template<typename T>
	void parse_input(char** argv, int argc, int& iarg, T& param)
	{
		if (iarg + 1 < argc)
		{
			T _val = convert_char<T>(argv[iarg + 1]);
			param = _val;
			iarg += 2;
		}
		else
		{
			std::cerr << "Invalid input arguments" << std::endl;
		}
	}


	template<typename T>
	void parse_input(char** argv, int argc, int& iarg, T& param1, T& param2)
	{
		if (iarg + 2 < argc)
		{
			T _val1 = convert_char<T>(argv[iarg + 1]);
			param1 = _val1;
			T _val2 = convert_char<T>(argv[iarg + 2]);
			param2 = _val2;
			iarg += 3;
		}
		else
		{
			std::cerr << "Invalid input arguments" << std::endl;
		}
	}
};

#endif
