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



void generate_timing_info();

int main(int argc, char** argv)
{
	generate_timing_info();
}




void generate_timing_info()
{
	bounds bnds;
	bnds.x_min = -3.56;
	bnds.x_max = 1.77;
	bnds.y_min = -1.5;
	bnds.y_max = 1.5;
	int x_size = 1920;
	int y_size = 1080;

	int allocation_mode;

	int num_threads;
#pragma omp parallel
	{
		num_threads = omp_get_num_threads();
	}

	thread_config trd_cnfg_serial, trd_cnfg_x_meshes, trd_cnfg_y_meshes;

	trd_cnfg_x_meshes.threads_x = num_threads;
	trd_cnfg_x_meshes.threads_y = 1;
	trd_cnfg_y_meshes.threads_x = 1;
	trd_cnfg_y_meshes.threads_y = num_threads;

	std::map<std::string, double> timings;
	std::map<std::string, double> areas;
	std::string timing_area_info_file("timing-area.csv");



	double timing, area;
	allocation_mode = C_X_MAJOR;
	std::string info("1-C_X_MAJOR---serial");
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_serial, area);
	timings[info] = timing;
	areas[info] = area;
	info = "2-C_X_MAJOR---X_MESHES";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "3-C_X_MAJOR---Y_MESHES";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	allocation_mode = C_Y_MAJOR;
	info = "4-C_Y_MAJOR---X_MESHES";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "5-C_Y_MAJOR---Y_MESHES";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	allocation_mode = CPP_X_MAJOR;
	info = "6-CPP_X_MAJOR---serial";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_serial, area);
	timings[info] = timing;
	areas[info] = area;
	info = "7-CPP_X_MAJOR---X_MESHES";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "8-CPP_X_MAJOR---Y_MESHES";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	allocation_mode = CPP_Y_MAJOR;
	info = "9-CPP_Y_MAJOR---X_MESHES";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "10-CPP_Y_MAJOR---Y_MESHES";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	allocation_mode = C_X_MAJOR;
	info = "11-C_X_MAJOR---X_MESHES---OMP_INNER";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "12-C_X_MAJOR---Y_MESHES---OMP_INNER";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	allocation_mode = C_Y_MAJOR;
	info = "13-C_Y_MAJOR---X_MESHES---OMP_INNER";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "14-C_Y_MAJOR---Y_MESHES---OMP_INNER";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	allocation_mode = CPP_X_MAJOR;
	info = "15-CPP_X_MAJOR---X_MESHES---OMP_INNER";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "16-CPP_X_MAJOR---Y_MESHES---OMP_INNER";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	allocation_mode = CPP_Y_MAJOR;
	info = "17-CPP_Y_MAJOR---X_MESHES---OMP_INNER";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "18-CPP_Y_MAJOR---Y_MESHES---OMP_INNER";
	timing = run_mandelbrot(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;


	writeMaps(timing_area_info_file, timings, areas);

	std::cout << "All done!" << std::endl;

	return;
}



