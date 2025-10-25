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


int main(int argc, char** argv)
{
	using Mandelbrot_NS::bounds;
	using Runner_NS::run_mandelbrot_timing;
	try {
		bounds bnds;
		bnds.x_min = -3.56;
		bnds.x_max = 1.77;
		bnds.y_min = -1.5;
		bnds.y_max = 1.5;
		int x_size = 1920;
		int y_size = 1080;
		auto run_mandelbrot_ptr = std::make_unique<run_mandelbrot_timing>(bnds, x_size, y_size);
		run_mandelbrot_ptr->run();
	}
	catch (std::bad_alloc &ae) {
		std::cout << "C++ allocation failed:" << ae.what() << std::endl;
		exit(1);
	}
	catch (std::exception& ae) {
		std::cout << "Exception: " << ae.what() << std::endl;
		exit(1);
	}

	return 0;
}




