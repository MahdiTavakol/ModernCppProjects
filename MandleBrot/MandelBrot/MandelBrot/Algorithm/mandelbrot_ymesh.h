#ifndef MANDELBROT_YMESH_H
#define MANDELBROT_YMESH_H

#include <omp.h>
#include "mandelbrot.h"

namespace Mandelbrot_NS {
	class mandelbrot_ymesh : public mandelbrot
	{
	public:
		mandelbrot_ymesh(
			/* allocation config */ const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
			/* space config */ const bounds& _bnds, const int& _n_xs, const int& _n_ys,
			/* run config */ const thread_config& thread_cfg_ = thread_config(),
			/* file name */ const std::string& _file_name = "Mandelbrot.dat");

		mandelbrot_ymesh(const mandelbrot_ymesh& other) = delete;
		mandelbrot_ymesh& operator=(const mandelbrot_ymesh& other) = delete;
		mandelbrot_ymesh(mandelbrot_ymesh&& other) = default;
		mandelbrot_ymesh& operator=(mandelbrot_ymesh&& other) = default;
		~mandelbrot_ymesh() = default;

		virtual void calculate(const double& scale) override = 0;

	protected:
		int n_threads_x, n_threads_y;
		int omp_num_threads, thread_id, size_per_thread;
		std::unique_ptr<int[]> first_ranges, last_ranges;

	};
}


#endif