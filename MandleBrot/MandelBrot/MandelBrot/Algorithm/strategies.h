#pragma once
#include "mandelbrot.h"
#include <functional>

using namespace Numerical_NS;

namespace Mandelbrot_NS {

	class omp_strategy {
	public:
			omp_strategy(mandelbrot& mndlbrt_ref_):
				mndlbrt_ref{ mndlbrt_ref_ }
			{};
			virtual ~omp_strategy() = default;
			virtual void dummy() = 0;
			virtual void calculate(const double& scale_);

	protected:
		int n_xs, n_ys,  x_min, x_max, y_min, y_max;
		int num_iterations;

		thread_config thread_cfg;
		std::unique_ptr<formula<double>> frmla;

		std::reference_wrapper<mandelbrot> mndlbrt_ref;
	};

	class omp_strategy_i : public omp_strategy {
	public: 
		omp_strategy_i(mandelbrot& mndlbrt_ref_):
			omp_strategy{mndlbrt_ref_}
		{}
		~omp_strategy_i() = default;
		void dummy() override {}
		void calculate(const double& scale_) override;
	};

	class omp_strategy_j : public omp_strategy {
	public:
		omp_strategy_j(mandelbrot& mndlbrt_ref_) :
			omp_strategy{ mndlbrt_ref_ }
		{}
		~omp_strategy_j() = default;
		void dummy() override {}
		void calculate(const double& scale_) override;
	};
}