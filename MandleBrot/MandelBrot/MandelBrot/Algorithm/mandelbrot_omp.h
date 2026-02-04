#pragma once

#include "mandelbrot.h"
#include <array>
#include <memory>

namespace Mandelbrot_NS {

	class mandelbrot_omp : public mandelbrot {
		mandelbrot_omp(/* allocation config */ const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
			           /* space config */ const bounds& _bnds, const int& _n_xs, const int& _n_ys,
			           /* thread configuration*/ const thread_config& thread_cfg,
			           /* file name */ const std::string& _file_name = "Mandelbrot.dat",
			           /* number of iterations*/ const int& num_iterations_=10000,
		               /* gamma */ const double& gamma = 1.0);

	private:
		

	};

}