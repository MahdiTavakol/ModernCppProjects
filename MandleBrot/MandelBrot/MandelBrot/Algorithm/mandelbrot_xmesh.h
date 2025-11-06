#ifndef MANDELBROT_XMESH_H
#define MANDELBROT_XMESH_H

#include <omp.h>
#include "mandelbrot.h"

namespace Mandelbrot_NS {

	class mandelbrot_xmesh : public mandelbrot
	{
	public:
		mandelbrot_xmesh(/* allocation config */ const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
			             /* space config */ const bounds& _bnds, const int& _n_xs, const int& _n_ys,
						 /* run config */ const thread_config& thread_cfg_ = thread_config(),
			             /* file name */ const std::string& _file_name = "Mandelbrot.dat",
			             /* number of iterations*/ const int& num_iterations_=10000,
			             /* gamma */ const double& gamma = 1.0);

		mandelbrot_xmesh(const mandelbrot_xmesh& _other) = delete;
		mandelbrot_xmesh& operator=(const mandelbrot_xmesh& _other) = delete;
		mandelbrot_xmesh(mandelbrot_xmesh&& _other) = default;
		mandelbrot_xmesh& operator=(mandelbrot_xmesh&& _other) = default;
		~mandelbrot_xmesh() = default;



		virtual void calculate(const double& _scale) override = 0;


	protected:
		int n_threads_x, n_threads_y;
		int omp_num_threads, thread_id, size_per_thread;
		std::unique_ptr<int[]> first_ranges, last_ranges;
	};
}

#endif
