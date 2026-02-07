#pragma once
#include "mandelbrot.h"
#include "../definitions.h"
#include "formula.h"
#include <functional>

using namespace Numerical_NS;

namespace Mandelbrot_NS {
	class mandelbrot;

	class omp_strategy {
	public:
			omp_strategy(mandelbrot& mndlbrt_ref_):
				mndlbrt_ref{ mndlbrt_ref_ }
			{};
			virtual ~omp_strategy() = default;
			virtual void dummy() = 0;
			virtual void calculate(const double& scale_);

	protected:
		int n_xs = 0, n_ys = 0;
		double x_min = 0.0, x_max = 0.0, y_min = 0.0, y_max = 0.0;
		int num_iterations = 10000;

		thread_config thread_cfg;
		formula<double>* frmlaRaw =  nullptr;

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

	class omp_strategy_serial : public omp_strategy {
		public:
		omp_strategy_serial(mandelbrot& mndlbrt_ref_) :
			omp_strategy{ mndlbrt_ref_ }
		{}
		~omp_strategy_serial() = default;
		void dummy() override {}
		void calculate(const double& scale_) override;
	};
}