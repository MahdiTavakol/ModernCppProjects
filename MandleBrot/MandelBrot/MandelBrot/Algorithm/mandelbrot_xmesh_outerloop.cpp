#include "mandelbrot_xmesh_outerloop.h"

using namespace Mandelbrot_NS;
using Numerical_NS::complex;


mandelbrot_xmesh_outerloop::mandelbrot_xmesh_outerloop(
/* allocation config */ const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
/* space config */ const bounds& _bnds, const int& _n_xs, const int& _n_ys,
/* run config */ const thread_config& thread_cfg_,
/* file name */ const std::string& _file_name,
/* number of iterations*/ const int& num_iterations_,
/* gamma */ const double& gamma_) :
mandelbrot_xmesh{ _alloc_mode,_alloc_major, _bnds, _n_xs, _n_ys,
thread_cfg_, _file_name, num_iterations_,gamma_}
{}


void mandelbrot_xmesh_outerloop::calculate(const double& _scale)

{
	double ara = 0.0;
#pragma omp parallel reduction(+:ara) // default(none) shared(n_xs,x_min,x_max,y_min,y_max,y_size,num_iterations,X,first_range,last_range,size_per_thread)
	{
		int thread_id = omp_get_thread_num();
		if (thread_id == 0)
			std::cout << "Using " << omp_get_num_threads() << " omp threads" << std::endl;
		int first = first_ranges[thread_id];
		int last = last_ranges[thread_id];
		for (int i = first; i < last; i++)
		{
			for (int j = 0; j < n_ys; j++)
			{
				complex min(static_cast<double> (this->x_min), static_cast<double> (this->y_min));
				double _i = static_cast<double> (i % this->n_xs);
				double _j = static_cast<double> (j);
				_i = static_cast<double> ((_i + this->n_xs / _scale) / _scale);
				_j = static_cast<double> ((_j + this->n_ys / _scale) / _scale);

				complex delta((double)(_i * (this->x_max - this->x_min) / (this->n_xs - 1)), (double)(_j * (this->y_max - this->y_min) / (this->n_ys - 1)));
				complex c = min + delta;
				complex z(0.0);


				int k = 0;
				for (k = 0; k < this->num_iterations; k++)
				{
					z = z * z + c;
					if (z.abs_complex() > escape) break;
				}
				if (k == this->num_iterations)
				{
					ara++;
				}
				(*(array_alloc_ptr))(i, j) = static_cast<double>(k) / static_cast<double>(this->num_iterations);
			}
		}
	}
	area = ara;
	for (int k = 0; k < n_threads_x; k++)
		for (int i = first_ranges[k]; i < last_ranges[k]; i++)
			for (int j = 0; j < n_ys; j++)
				(*(array_alloc_ptr))(i - k * this->n_xs, j) = (*(array_alloc_ptr))(i, j);
}
