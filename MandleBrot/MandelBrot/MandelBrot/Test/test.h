#pragma once
#include <vector>
#include <string>

#include <memory>

#include "../Runner/run_mandelbrot.h"
#include "../Runner/run_mandelbrot_factory.h"
using Runner_NS::run_mandelbrot_factory;
using Runner_NS::run_mandelbrot;


using std::vector, std::string;

class test_args_builder {
public:
	test_args_builder(
		string dummy_ = "test",
		string mode_ = "single",
		string type_ = "1",
		string info_ = "test",
		string res_x_ = "198",
		string res_y_ = "108",
		string threads_x_ = "1",
		string threads_y_ = "1",
		string others_ = ""):
		dummy{dummy_},
		mode{mode_},
		type{type_},
		info{info_},
		res_x{res_x_},
		res_y{res_y_},
		threads_x{threads_x_},
		threads_y{threads_y_},
		others{others_} 
	{}

	vector<string> operator()() const {
		vector<string> args;
		args.push_back(dummy);
		args.push_back(mode);
		args.push_back("--type");
		args.push_back(type);
		args.push_back("--info");
		args.push_back(info);
		args.push_back("--resolution");
		args.push_back(res_x);
		args.push_back(res_y);
		args.push_back("--threads");
		args.push_back(threads_x);
		args.push_back(threads_y);
		if (!others.empty())
			args.push_back(others);
		return args;
	}


private:
	string dummy, mode, type, info, res_x, res_y;
	string threads_x, threads_y, others;
}; 

int run_wrapper(const vector<string>& args)
{
	auto factory = std::make_unique<run_mandelbrot_factory>(args);
	std::unique_ptr<run_mandelbrot> run_mandelbrot_ptr;
	factory->return_runner(run_mandelbrot_ptr);
	run_mandelbrot_ptr->run();
	int area = run_mandelbrot_ptr->return_area();
	return area;
}