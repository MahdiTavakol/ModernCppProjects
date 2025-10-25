#include "mandelbrot_ymesh.h"

using namespace Mandelbrot_NS;
using Numerical_NS::complex;

mandelbrot_ymesh::mandelbrot_ymesh(/* allocation config */ const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
	/* space config */ const bounds& _bnds, const int& _n_xs, const int& _n_ys,
	/* run config */ const thread_config& thread_cfg_,
	/* file name */ const std::string& _file_name) :
	mandelbrot{ _alloc_mode,_alloc_major, _bnds, _n_xs, _n_ys, _file_name },
	n_threads_x{ thread_cfg_.threads_x }, n_threads_y{ thread_cfg_.threads_y }
{
	omp_set_num_threads(n_threads_y);
#pragma omp parallel
	{
		omp_num_threads = omp_get_num_threads();
		int thread_id = omp_get_thread_num();
#pragma omp single
		{
			std::cout << "Initiating " << omp_num_threads << " omp threads " << std::endl;
			first_ranges = std::make_unique<int []>(omp_num_threads * sizeof(int));
			last_ranges = std::make_unique<int[]>(omp_num_threads * sizeof(int));
		}
		size_per_thread = (this->n_ys + omp_num_threads - 1) / omp_num_threads;
		int first = thread_id * this->n_ys + thread_id * size_per_thread;
		int last = first + size_per_thread;
		if (last >= this->n_ys * omp_num_threads) last = this->n_ys * omp_num_threads;
		first_ranges[thread_id] = first;
		last_ranges[thread_id] = last;
	}
}


