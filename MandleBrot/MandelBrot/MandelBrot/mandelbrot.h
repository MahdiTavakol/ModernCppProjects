#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <string>
#include <fstream>
#include <memory>

#include "complex.h"
#include "array_allocator.h"
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
	mandelbrot(const allocation_mode& _alloc_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys);
	mandelbrot(const allocation_mode& _alloc_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const thread_config& _thread_config);
	mandelbrot(const allocation_mode& _alloc_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const std::string& _file_name);
	mandelbrot(const allocation_mode& _alloc_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const thread_config& _thread_config, const std::string& _file_name);
	mandelbrot(const allocation_mode& _alloc_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const int& _n_threads_x, const int& _n_threads_y, const std::string& _file_name);
	~mandelbrot();
	virtual void calculate();
	virtual void calculate(const double& _scale);
	void output();
	double& return_area();

protected:
	allocation_mode alloc_mode;
	int n_xs, n_ys;
	int n_threads_x, n_threads_y;
	double x_min, x_max, y_min, y_max;
	double area;
	int num_iterations;
	std::unique_ptr<array_allocator> array_alloc_ptr;

	std::string file_name;
	std::unique_ptr<std::ofstream> file;
};


#endif
