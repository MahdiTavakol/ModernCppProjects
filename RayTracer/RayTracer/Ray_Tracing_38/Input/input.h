#ifndef INPUT_H
#define INPUT_H

#include <fstream>
#include <map>
#include <variant>
#include "../Algorithms/camera.h"
#include "settings.h"
#include "app_settings.h"


class input
{
public:
	input(int argc, char** argv, int mode_,
		std::map<std::string, int> app_set_map_,
		communicator* para_,
		std::vector<std::reference_wrapper<std::ostream>> strmVec_ = {
			std::vector<std::reference_wrapper<std::ostream>>{
				std::ref(std::cout)
			}
		});

	std::unique_ptr<app_settings> return_app_settings();


	// since a communicator object is required to 
	// create the input class we consider settings
	// for a communicator as a special case
	static void set_communicator_settings(int argc, char** argv, settings* com_settings);

	// parsing the input file
	void parse_file();


protected:
	int mode;
	template<typename T>
	T convert_char(char* _chr);
	communicator* para;


	std::vector<std::reference_wrapper<std::iostream>> outStreams;
	void initialize(int argc, char** argv);
	void logger_function(int argc, char** argv);


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
	void fill_iostream(int argc, char** argv);
	// some options are not compatible so this checks their compatibility
	void check_compatibility();




private:
	std::unique_ptr<std::iostream> input_stream;
	std::unique_ptr<app_settings> app_set;
	std::map<std::string, int> app_set_map;
};

#endif
