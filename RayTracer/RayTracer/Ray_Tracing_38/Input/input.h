#ifndef INPUT_H
#define INPUT_H

#include <fstream>
#include <map>
#include <variant>
#include "../Algorithms/camera.h"
#include "settings.h"
#include "camera_settings.h"
#include "app_settings.h"
#include "image_settings.h"
#include "output_settings.h"
#include "scene_settings.h"


class input
{
public:
	input(int argc, char** argv, 
		std::map<std::string, int> app_set_map,
		communicator* para_,
		std::vector<std::reference_wrapper<std::ostream>> strmVec_ = {
			std::vector<std::reference_wrapper<std::ostream>>{
				std::ref(std::cout)
			}
		});

	std::unique_ptr<app_settings> return_app_settings();


protected:
	template<typename T>
	T convert_char(char* _chr);
	communicator* para;


	std::vector<std::reference_wrapper<std::ostream>> outStreams;
	void initialize(int argc, char** argv);
	void logger_function(int argc, char** argv);

	// parsing the input file
	void parse_file();
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


	void init_app_settings();
	void parse_render_mode(std::stringstream& ss);
	void fill_iostream(int argc, char** argv);
	bool parse_render_mode(char** argv, int argc, int& iarg);



private:
	std::unique_ptr<std::iostream> input_stream;
	std::unique_ptr<app_settings> app_set;
	std::map<std::string, int> app_set_map;
};

#endif
