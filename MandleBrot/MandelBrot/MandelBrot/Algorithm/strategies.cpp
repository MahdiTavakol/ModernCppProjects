#include "strategies.h"
#include <omp.h>
#include <memory>
#include <array>

#include "../definitions.h"

using namespace Mandelbrot_NS;

void omp_strategy::calculate(const double& _scale)
{
	n_xs = mndlbrt_ref.get().n_xs;
	n_ys = mndlbrt_ref.get().n_ys;
	x_min = mndlbrt_ref.get().x_min;
	x_max = mndlbrt_ref.get().x_max;
	y_min = mndlbrt_ref.get().y_min;
	y_max = mndlbrt_ref.get().y_max;

	num_iterations = mndlbrt_ref.get().num_iterations;

	thread_cfg = mndlbrt_ref.get().thread_cfg;

	frmlaRaw = mndlbrt_ref.get().frmla.get();

}


void omp_strategy_i::calculate(const double& _scale)
{
	omp_strategy::calculate(_scale);

	auto& array_alloc_ptr = mndlbrt_ref.get().array_alloc_ptr;
	auto& area = mndlbrt_ref.get().area;

	auto& x_ranges = mndlbrt_ref.get().x_ranges;
	auto& y_ranges = mndlbrt_ref.get().y_ranges;

	double ara = 0.0;
#pragma omp parallel reduction(+:ara) // default(none) shared(n_xs,x_min,x_max,y_min,y_max,y_size,num_iterations,X,first_range,last_range,size_per_thread)
	{
		int thread_id = omp_get_thread_num();
		if (thread_id == 0)
			std::cout << "Using " << omp_get_num_threads() << " omp threads" << std::endl;




		const int& first_x = x_ranges[thread_id][0];
		const int& last_x = x_ranges[thread_id][1];
		const int& first_y = y_ranges[thread_id][0];
		const int& last_y = y_ranges[thread_id][1];


		for (int i = first_x; i < last_x; i++)
		{
			for (int j = first_y; j < last_y; j++)
			{
				complex<double> min(static_cast<double>(x_min), static_cast<double>(y_min));
				double _i = static_cast<double>(i % n_xs);
				double _j = static_cast<double>(j % n_ys);
				_i = static_cast<double>((_i + n_xs / _scale) / _scale);
				_j = static_cast<double>((_j + n_ys / _scale) / _scale);

				complex<double> delta((double)(_i * (x_max - x_min) / (n_xs - 1)), (double)(_j * (y_max - y_min) / (n_ys - 1)));
				complex<double> c = min + delta;
				complex<double> z(0.0);


				int k = 0;
				for (k = 0; k < num_iterations; k++)
				{
					z = (*frmlaRaw)(z, c);
					if (z.abs_complex() > escape) break;
				}
				if (k == num_iterations)
				{
					ara++;
				}
				(*(array_alloc_ptr))(i, j) = static_cast<double>(k) / static_cast<double>(num_iterations);
			}
		}
	}
	area = ara;
	for (int k = 0; k < thread_cfg.threads_x; k++)
		for (int l = 0; l < thread_cfg.threads_y; l++)
			for (int i = x_ranges[k][0]; i < x_ranges[k][1]; i++)
				for (int j = y_ranges[l][0]; j < y_ranges[l][1]; j++)
					(*(array_alloc_ptr))(i - k * n_xs, j - l * n_ys) = (*(array_alloc_ptr))(i, j);
}

void omp_strategy_j::calculate(const double& _scale)
{
	omp_strategy::calculate(_scale);

	auto& array_alloc_ptr = mndlbrt_ref.get().array_alloc_ptr;
	auto& area = mndlbrt_ref.get().area;

	auto& x_ranges = mndlbrt_ref.get().x_ranges;
	auto& y_ranges = mndlbrt_ref.get().y_ranges;

	double ara = 0.0;
#pragma omp parallel reduction(+:ara) // default(none) shared(n_xs,x_min,x_max,y_min,y_max,y_size,num_iterations,X,first_range,last_range,size_per_thread)
	{
		int thread_id = omp_get_thread_num();
		if (thread_id == 0)
			std::cout << "Using " << omp_get_num_threads() << " omp threads" << std::endl;




		const int& first_x = x_ranges[thread_id][0];
		const int& last_x = x_ranges[thread_id][1];
		const int& first_y = y_ranges[thread_id][0];
		const int& last_y = y_ranges[thread_id][1];


		for (int j = first_y; j < last_y; j++)
		{
			for (int i = first_x; i < last_x; i++)
			{
				complex min(static_cast<double>(x_min), static_cast<double>(y_min));
				double _i = static_cast<double>(i % n_xs);
				double _j = static_cast<double>(j % n_ys);
				_i = static_cast<double>((_i + n_xs / _scale) / _scale);
				_j = static_cast<double>((_j + n_ys / _scale) / _scale);

				complex delta((double)(_i * (x_max - x_min) / (n_xs - 1)), (double)(_j * (y_max - y_min) / (n_ys - 1)));
				complex c = min + delta;
				complex z(0.0);


				int k = 0;
				for (k = 0; k < num_iterations; k++)
				{
					z = (*frmlaRaw)(z, c);
					if (z.abs_complex() > escape) break;
				}
				if (k == num_iterations)
				{
					ara++;
				}
				(*(array_alloc_ptr))(i, j) = static_cast<double>(k) / static_cast<double>(num_iterations);
			}
		}
	}
	area = ara;
	for (int k = 0; k < thread_cfg.threads_x; k++)
		for (int l = 0; l < thread_cfg.threads_y; l++)
			for (int i = x_ranges[k][0]; i < x_ranges[k][1]; i++)
				for (int j = y_ranges[l][0]; j < y_ranges[l][1]; j++)
					(*(array_alloc_ptr))(i - k * n_xs, j - l * n_ys) = (*(array_alloc_ptr))(i, j);
}