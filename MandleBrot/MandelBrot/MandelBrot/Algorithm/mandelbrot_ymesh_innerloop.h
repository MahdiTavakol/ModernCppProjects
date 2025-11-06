#ifndef MANDELBROT_YMESH_INNERLOOP_H
#define MANDELBROT_YMESH_INNERLOOP_H

#include <omp.h>
#include "mandelbrot_ymesh.h"

namespace Mandelbrot_NS {
	class mandelbrot_ymesh_innerloop : public mandelbrot_ymesh
	{
	public:
		mandelbrot_ymesh_innerloop(/* allocation config */ const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
			/* space config */ const bounds& _bnds, const int& _n_xs, const int& _n_ys,
			/* run config */ const thread_config& thread_cfg_ = thread_config(),
			/* file name */ const std::string& _file_name = "Mandelbrot.dat",
			/* number of iterations*/ const int& num_iterations_ = 10000,
			/* gamma */ const double& gamma_=1.0);

		void calculate(const double& _scale);

	};
}


#endif