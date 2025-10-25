#include <cmath>
#include <iostream>
#include <fstream>
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
#include "Algorithm/run_mandelbrot.h"


int main(int argc, char** argv)
{
	using Mandelbrot_NS::bounds;
	bounds bnds;
	bnds.x_min = -3.56;
	bnds.x_max = 1.77;
	bnds.y_min = -1.5;
	bnds.y_max = 1.5;
	int x_size = 1920;
	int y_size = 1080;
	auto run_mandelbrot_ptr = std::make_unique<run_mandelbrot<Run_type::TIMING>>(bnds, x_size, y_size);
	run_mandelbrot_ptr->run();

	return 0;
}




