#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <string>
#include <fstream>
#include <memory>

#include "../Numerical/complex.h"
#include "../Array/array_allocator.h"
#include "../definitions.h"

using Array_NS::allocation_mode;
using Array_NS::array_allocator;
using Array_NS::allocation_major;

constexpr double escape = 1.0;

namespace Mandelbrot_NS {

	struct bounds {
		double x_min;
		double x_max;
		double y_min;
		double y_max;
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
		void output();
		double& return_area();

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
	};
}

#endif

