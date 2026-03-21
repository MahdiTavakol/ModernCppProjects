#include "mandelbrot.h"
#include "../Numerical/complex.h"
#include "../Array/array.h"
#include <array>

using namespace Mandelbrot_NS;
using Numerical_NS::complex;

mandelbrot::mandelbrot(const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
	                   const bounds& _bnds, 
	                   std::array<int,2> _res,
	                   const std::string& _file_name,
	                   const int& num_iterations_,
					   const double& gamma_):
	alloc_mode{ _alloc_mode }, alloc_major{_alloc_major},
	gamma{gamma_},
	resolution{_res},
	x_min {_bnds.x_min}, x_max{ _bnds.x_max }, y_min{ _bnds.y_min }, y_max{ _bnds.y_max },
	num_iterations{num_iterations_},
	file_name{ _file_name }
{
	array_alloc_ptr = std::make_unique<array_allocator>(alloc_mode, alloc_major, resolution[0], resolution[1], file_name, gamma_);
	this->area = 0;
}


void mandelbrot::calculate(const double& _scale)
{
	int n_xs = resolution[0];
	int n_ys = resolution[1];
	// Row bases similar to our allocation
	for (int j = 0; j < n_ys; j++)
		for (int i = 0; i < n_xs; i++)
		{
			complex min(x_min, y_min);
			double _i = static_cast<double> ((i + n_xs / _scale) / _scale);
			double _j = static_cast<double> ((j + n_ys / _scale) / _scale);

			complex delta(static_cast<double>(_i * (x_max - x_min) / (n_xs - 1)), static_cast<double>(_j * (y_max - y_min) / (n_ys - 1)));
			complex c = min + delta;
			complex z(0.0);


			int k = 0;
			for (k = 0; k < num_iterations; k++)
			{
				z = z * z + c;
				if (z.abs_complex() > 4.0) break;
			}
			if (k == num_iterations)
			{
				area++;
			}
			(*(array_alloc_ptr))(i, j) = static_cast<double>(k) / static_cast<double>(this->num_iterations);
		}
}

void mandelbrot::output()
{
	array_alloc_ptr->write_data(resolution[0],resolution[1]);
}

int& mandelbrot::return_area()
{
	return this->area;
}




