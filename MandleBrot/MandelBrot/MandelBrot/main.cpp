#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <map>

#include <omp.h>

#include "definitions.h"
#include "Array/array.h"
#include "Numerical/complex.h"
#include "memory.h"
#include "Algorithm/mandelbrot.h"
#include "Algorithm/mandelbrot_xmesh.h"
#include "Algorithm/mandelbrot_ymesh.h"
#include "Algorithm/mandelbrot_xmesh_innerloop.h"
#include "Algorithm/mandelbrot_ymesh_innerloop.h"
#include "Runner/run_mandelbrot.h"
#include "Runner/run_mandelbrot_timing.h"
#include "Runner/run_mandelbrot_animation.h"

using Mandelbrot_NS::bounds;
using Runner_NS::run_mandelbrot;
using Runner_NS::run_mandelbrot_timing;
using Runner_NS::run_mandelbrot_animation;

void timing(std::unique_ptr<run_mandelbrot>&);
void animation(std::unique_ptr<run_mandelbrot>&);


int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	try {
		std::unique_ptr<run_mandelbrot> run_mandelbrot_ptr;
		animation(run_mandelbrot_ptr);
		run_mandelbrot_ptr->run();
	}
	catch (std::bad_alloc &ae) {
		std::cerr << "C++ allocation failed:" << ae.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception& ae) {
		std::cerr << "Exception: " << ae.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void timing(std::unique_ptr<run_mandelbrot>& run_mandelbrot_ptr)
{
	bounds bnds;
	bnds.x_min = -3.56;
	bnds.x_max = 1.77;
	bnds.y_min = -1.5;
	bnds.y_max = 1.5;
	int x_size = 1920;
	int y_size = 1080;
	run_mandelbrot_ptr = std::make_unique<run_mandelbrot_timing>(bnds, x_size, y_size);
	run_mandelbrot_ptr->run();
}

void animation(std::unique_ptr<run_mandelbrot>& run_madelbrot_ptr)
{
	std::string info("Animate_1");
	bounds bnds;
	bnds.x_min = -3.56;
	bnds.x_max = 1.77;
	bnds.y_min = -1.5;
	bnds.y_max = 1.5;
	int x_size = 1920;
	int y_size = 1080;
	thread_config tcfg;
#pragma omp parallel 
	{
		tcfg.threads_x = omp_get_num_threads();
		tcfg.threads_y = 1;
	}
	run_madelbrot_ptr = std::make_unique<run_mandelbrot_animation>(info, bnds, x_size, y_size,
		tcfg, Runner_NS::Mesh_type::XMESH_OUTER_LOOP);

}




