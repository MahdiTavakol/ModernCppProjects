#pragma once
#include <vector>
#include <string>

#include <memory>

#include "../Runner/run_mandelbrot.h"
#include "../Runner/run_mandelbrot_factory.h"
using Runner_NS::run_mandelbrot_factory;
using Runner_NS::run_mandelbrot;


using std::vector, std::string;

double run_wrapper(const vector<string>& args)
{
	auto factory = std::make_unique<run_mandelbrot_factory>(args);
	std::unique_ptr<run_mandelbrot> run_mandelbrot_ptr;
	factory->return_runner(run_mandelbrot_ptr);
	run_mandelbrot_ptr->run();
	double output = run_mandelbrot_ptr->return_area();
	return output;
}