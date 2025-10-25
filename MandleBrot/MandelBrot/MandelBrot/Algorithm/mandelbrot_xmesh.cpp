#include "mandelbrot_xmesh.h"

using namespace Mandelbrot_NS;
using Numerical_NS::complex;

mandelbrot_xmesh::mandelbrot_xmesh(const allocation_mode& _alloc_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys) :
	mandelbrot_xmesh(_alloc_mode, _bnds, _n_xs, _n_ys, (int)1, (int)1,  std::string("Mandelbrot_xmesh.dat"))
{}

mandelbrot_xmesh::mandelbrot_xmesh(const allocation_mode& _alloc_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const thread_config& _thread_config) :
		mandelbrot_xmesh(_alloc_mode, _bnds, _n_xs, _n_ys, _thread_config, std::string("Mandelbrot_xmesh.dat"))
	{}

mandelbrot_xmesh::mandelbrot_xmesh(const allocation_mode& _alloc_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys, const std::string& _file_name) :
mandelbrot_xmesh(_alloc_mode, _bnds, _n_xs, _n_ys, (int)1, (int)1, std::string("Mandelbrot_xmesh.dat")) {}

mandelbrot_xmesh::mandelbrot_xmesh(const allocation_mode& _alloc_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys,
	const thread_config& _thread_config, const std::string& _file_name) :
	mandelbrot_xmesh(_alloc_mode, _bnds, _n_xs, _n_ys, _thread_config.threads_x, (int)1, _file_name)
{}

mandelbrot_xmesh::mandelbrot_xmesh(const allocation_mode& _alloc_mode, const bounds& _bnds, const int& _n_xs, const int& _n_ys,
	const int& _n_threads_x, const int& _n_threads_y, const std::string& _file_name) :
		mandelbrot(_alloc_mode, _bnds, _n_xs, _n_ys, _n_threads_x, _n_threads_y, _file_name)
	{
		omp_set_num_threads(n_threads_x);
#pragma omp parallel
		{
			omp_num_threads = omp_get_num_threads();
			int thread_id = omp_get_thread_num();
#pragma omp single
			{
				std::cout << "Initiating " << omp_num_threads << " omp threads " << std::endl;
				first_ranges = std::make_unique<int []>(omp_num_threads * sizeof(int));
				last_ranges = std::make_unique<int[]>(omp_num_threads * sizeof(int));
			}
			size_per_thread = 1 + std::floor(this->n_xs / omp_num_threads);
			int first = thread_id * this->n_xs + thread_id * size_per_thread;
			int last = first + size_per_thread;
			if (last >= this->n_xs * omp_num_threads) last = this->n_xs * omp_num_threads;
			first_ranges[thread_id] = first;
			last_ranges[thread_id] = last;
		}
	}

