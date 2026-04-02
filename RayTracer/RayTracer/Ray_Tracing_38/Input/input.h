#ifndef INPUT_H
#define INPUT_H

#include <fstream>
#include <map>
#include <variant>
#include "../Algorithms/camera.h"
#include "../Algorithms/camera_settings.h"


class input
{
public:
	input(int argc, char** argv, 
		std::vector<std::reference_wrapper<std::ostream>> strmVec_ =
		{ 
			std::vector<std::reference_wrapper<std::ostream>>{
				std::ref(std::cout)
			} 
		});
	input(int argc, char** argv, int _image_width, int _samples_per_pixel,
		int _max_depth, int _vfov, double _width_ratio, double _height_ratio,
		camera_settings* cam_settings_);
	input(int argc, char** argv, camera_settings* cam_settings,
		std::vector<std::reference_wrapper<std::ostream>> strmVec_ = {
			std::vector<std::reference_wrapper<std::ostream>>{
				std::ref(std::cout)
			}
		}
	);


protected:
	template<typename T>
	T convert_char(char* _chr);
	camera_settings* cam_settings;


	bool input_logger;
	std::ofstream logfile;
	std::vector<std::reference_wrapper<std::ostream>> outStreams;
	void logger_function(int argc, char** argv);


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
