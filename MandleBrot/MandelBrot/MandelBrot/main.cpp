#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
#include <map>

#include <omp.h>

#include "definitions.h"
#include "array.h"
#include "complex.h"
#include "memory.h"
#include "mandelbrot.h"
#include "mandelbrot_xmesh.h"
#include "mandelbrot_ymesh.h"
#include "mandelbrot_xmesh_innerloop.h"
#include "mandelbrot_ymesh_innerloop.h"
#include "run_mandelbrot.h"


int main(int argc, char** argv)
{
	bounds bnds;
	bnds.x_min = -3.56;
	bnds.x_max = 1.77;
	bnds.y_min = -1.5;
	bnds.y_max = 1.5;
	int x_size = 1920;
	int y_size = 1080;
	auto run_mandelbrot_ptr = std::make_unique<run_mandelbrot<Run_type::TIMING>>(bnds, x_size, y_size);
	run_mandelbrot_ptr->run();

	return;
}




