#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <string>
#include <fstream>

#include "complex.h"
#include "array.h"
#include "definitions.h"


struct bounds {
	double x_min;
	double x_max;
	double y_min;
	double y_max;
};


class mandelbrot
{
public:
	mandelbrot(const int& _allocation_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys);
	mandelbrot(const int& _allocation_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const thread_config& _thread_config);
	mandelbrot(const int& _allocation_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const std::string& _file_name);
	mandelbrot(const int& _allocation_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const thread_config& _thread_config, const std::string& _file_name);
	mandelbrot(const int& _allocation_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const int& _n_threads_x, const int& _n_threads_y, const std::string& _file_name);
	~mandelbrot();
	void calculate();
	void output();
	double& return_area();

protected:
	int allocation_mode;
	int n_xs, n_ys;
	int n_threads_x, n_threads_y;
	double x_min, x_max, y_min, y_max;
	double area;
	int num_iterations;
	array* img;

	std::string file_name;
	std::ofstream* file;
};


#endif
