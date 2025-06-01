#include "mandelbrot_ymesh.h"


mandelbrot_ymesh::mandelbrot_ymesh(const int& _allocation_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys)
	: mandelbrot_ymesh(_allocation_mode, _bnds, _n_xs, _n_ys, (int)1, (int)1, std::string("Mandelbrot_ymesh.dat"))
{}
mandelbrot_ymesh::mandelbrot_ymesh(const int& _allocation_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const thread_config& _thread_config) :
	mandelbrot_ymesh(_allocation_mode, _bnds, _n_xs, _n_ys, _thread_config, std::string("Mandelbrot_ymesh.dat"))
{}
mandelbrot_ymesh::mandelbrot_ymesh(const int& _allocation_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const std::string& _file_name) :
	mandelbrot_ymesh(_allocation_mode, _bnds, _n_xs, _n_ys, (int)1, (int)1, _file_name)
{}
mandelbrot_ymesh::mandelbrot_ymesh(const int& _allocation_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, 
	const thread_config& _thread_config, const std::string& _file_name) :
	mandelbrot_ymesh(_allocation_mode, _bnds, _n_xs, _n_ys, _thread_config.threads_x,_thread_config.threads_y, _file_name)
{}

mandelbrot_ymesh::mandelbrot_ymesh(const int& _allocation_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, 
	const int& _n_threads_x, const int& _n_threads_y, const std::string& _file_name) :
	mandelbrot(_allocation_mode, _bnds, _n_xs, _n_ys,(int)1, _n_threads_y, _file_name)
{
	omp_set_num_threads(n_threads_y);
#pragma omp parallel
	{
		omp_num_threads = omp_get_num_threads();
		int thread_id = omp_get_thread_num();
#pragma omp single
		{
			std::cout << "Initiating " << omp_num_threads << " omp threads " << std::endl;
			first_range = (int*)malloc(omp_num_threads * sizeof(int));
			last_range = (int*)malloc(omp_num_threads * sizeof(int));
		}
		size_per_thread = 1 + std::floor(this->n_ys / omp_num_threads);
		int first = thread_id * this->n_ys + thread_id * size_per_thread;
		int last = first + size_per_thread;
		if (last >= this->n_ys * omp_num_threads) last = this->n_ys * omp_num_threads;
		first_range[thread_id] = first;
		last_range[thread_id] = last;
	}
}

void mandelbrot_ymesh::calculate(const double& _scale)
{
	double ara = 0.0;
#pragma omp parallel reduction(+:ara) // default(none) shared(n_xs,x_min,x_max,y_min,y_max,y_size,num_iterations,X,first_range,last_range,size_per_thread)
	{
		int thread_id = omp_get_thread_num();
		if (thread_id == 0)
			std::cout << "Using " << omp_get_num_threads() << " omp threads" << std::endl;
		int first = first_range[thread_id];
		int last = last_range[thread_id];
		for (int j = first; j < last; j++)
		{
			for (int i = 0; i < n_xs; i++)
			{
				complex min(static_cast<double> (this->x_min), static_cast<double> (this->y_min));
				double _i = static_cast<double> (i);
				double _j = static_cast<double> (j % this->n_ys);
				_i = static_cast<double> ((_i + this->n_xs / _scale) / _scale);
				_j = static_cast<double> ((_j + this->n_ys / _scale) / _scale);

				complex delta((double)(_i * (this->x_max - this->x_min) / (this->n_xs - 1)), (double)(_j * (this->y_max - this->y_min) / (this->n_ys - 1)));
				complex c = min + delta;
				complex z(0.0);


				int k = 0;
				for (k = 0; k < this->num_iterations; k++)
				{
					z = z * z + c;
					if (z.abs_complex() > 4.0) break;
				}
				if (k == this->num_iterations)
				{
					ara++;
				}
				(*(this->img))(i, j) = static_cast<double>(k) / static_cast<double>(this->num_iterations);
			}
		}
	}
	area = ara;		
	for (int k = 0; k < n_threads_y; k++)
		for (int j = first_range[k]; j < last_range[k]; j++)
			for (int i = 0; i < n_xs; i++)
				(*(this->img))(i, j - k * this->n_ys) = (*(this->img))(i, j);
}

void mandelbrot_ymesh::calculate()
{
	calculate(2.0);
}

