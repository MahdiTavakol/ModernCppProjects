#include "mandelbrot_omp.h"
#include <omp.h>

using namespace Mandelbrot_NS;

mandelbrot_omp::mandelbrot_omp(
	/* allocation config */ const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
	/* space config */ const bounds& _bnds, const int& _n_xs, const int& _n_ys,
	/* thread configuration*/ const thread_config& thread_cfg,
	/* file name */ const std::string& _file_name,
	/* number of iterations*/ const int& num_iterations_,
	/* gamma */ const double& gamma) :
	mandelbrot{ _alloc_mode, _alloc_major, _bnds, _n_xs, _n_ys, _file_name, num_iterations_, gamma }
{
	int threads_x = thread_cfg.threads_x;
	int threads_y = thread_cfg.threads_y;

	x_ranges = std::make_unique<std::array<int, 2>[]>(threads_x);
	y_ranges = std::make_unique<std::array<int, 2>[]>(threads_y);


#pragma omp parallel 
	{
		int num_threads = omp_get_num_threads();
		if (num_threads != threads_x * threads_y)
		{
			std::cout << "Warning: resettting the number of threads" << std::endl;
		}

	}

	omp_set_num_threads(threads_x * threads_y);

	int x_per_thread = (this->n_xs + threads_x - 1) / threads_x;
	for (int i = 0; i < threads_x; i++) {
		int x_first = i * this->n_xs + i * size_per_thread;
		int x_last = x_first + size_per_thread;
		if (x_last > this->n_xs * threads_x) x_last = this->n_xs * threads_x;
		x_ranges[i] = std::array<int, 2>{ x_first,x_last };
	}


	int y_per_thread = (this->n_ys + threads_y - 1) / threads_y;
	for (int i = 0; i < threads_y; i++) {
		int y_first = i * this->n_ys + i * size_per_thread;
		int y_last = y_first + size_per_thread;
		if (y_last > this->n_ys * threads_y) y_last = this->n_ys * threads_y;
		y_ranges[i] = std::array<int, 2>{ y_first,y_last };
	}




}