#pragma once

#include <omp.h>
#include "mandelbrot_xmesh.h"

namespace Mandelbrot_NS {
	class mandelbrot_xmesh_outerloop final : public mandelbrot_xmesh
	{
	public:
		mandelbrot_xmesh_outerloop(/* allocation config */ const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
			/* space config */ const bounds& _bnds, const int& _n_xs, const int& _n_ys,
			/* run config */ const thread_config& thread_cfg_ = thread_config(),
			/* file name */ const std::string& _file_name = "Mandelbrot.dat");

		void calculate(const double& _scale) override;
	};
}


