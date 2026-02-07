#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <string>
#include <fstream>
#include <memory>

#include "../Array/array_allocator.h"
#include "../definitions.h"
#include "formula.h"

using Array_NS::allocation_mode;
using Array_NS::array_allocator;
using Array_NS::allocation_major;

constexpr double escape = 2.0;

namespace Mandelbrot_NS {

	struct bounds {
		double x_min = -3.56;
		double x_max =  1.77;
		double y_min = -1.50;
		double y_max =  1.50;
	};


	class mandelbrot
	{
	public:
		mandelbrot(/* allocation config */ const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
			       /* space config */ const bounds& _bnds, const int& _n_xs, const int& _n_ys,
			       /* file name */ const std::string& _file_name = "Mandelbrot.dat",
			       /* number of iterations*/ const int& num_iterations_=10000,
		           /* gamma */ const double& gamma = 1.0);
		mandelbrot(const mandelbrot&) = delete;
		mandelbrot& operator=(const mandelbrot&) = delete;
		mandelbrot(mandelbrot&&) = default;
		mandelbrot& operator=(mandelbrot&&) = default;
 		virtual ~mandelbrot() = default;

		void calculate() { calculate(2.0); }
		virtual void calculate(const double& _scale);
		void reset(const bounds& bnds_, const std::string& file_name_)
		{
			x_min = bnds_.x_min; x_max = bnds_.x_max;
			y_min = bnds_.y_min; y_max = bnds_.y_max;
			array_alloc_ptr->reset(file_name_);
		}
		void output();
		double& return_area();



		friend class omp_strategy;
		friend class omp_strategy_i;
		friend class omp_strategy_j;
		friend class omp_strategy_serial;

	protected:
		allocation_mode alloc_mode;
		allocation_major alloc_major;
		double gamma;
		int n_xs, n_ys;
		double x_min, x_max, y_min, y_max;
		double area;
		int num_iterations;
		std::unique_ptr<array_allocator> array_alloc_ptr;
		std::string file_name;
		std::unique_ptr<std::ofstream> file;

		// Objects of strategy classes
		std::unique_ptr<formula<double>> frmla;
		


		// These are for the omp variant
		// In order to avoid circular dependency 
		// in the strategies I had to put 
		// these here.
		thread_config thread_cfg;
		int omp_num_threads = 0, size_per_thread = 0;
		std::unique_ptr<std::array<int, 2>[]> x_ranges, y_ranges;
	};
}

#endif

