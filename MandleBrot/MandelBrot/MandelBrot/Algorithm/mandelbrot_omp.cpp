#include "mandelbrot_omp.h"
#include <array>
#include <omp.h>

using namespace Mandelbrot_NS;

mandelbrot_omp::mandelbrot_omp(
	/* allocation config */ const allocation_mode& _alloc_mode, const allocation_major& _alloc_major,
	/* space config */ const bounds& _bnds,
	/* resolution */ std::array<int, 2> _res,
	/* thread configuration*/ const thread_config& thread_cfg_,
	/* mesh type */ const Mesh_type& mesh_type_,
	/* file name */ const std::string& _file_name,
	/* number of iterations*/ const int& num_iterations_,
	/* gamma */ const double& gamma,
	/* is it burning ship*/ const bool& burning_) :
	mandelbrot{ _alloc_mode, _alloc_major, _bnds, _res, _file_name, num_iterations_, gamma }
{
	thread_cfg = thread_cfg_;
	int threads_x = thread_cfg.threads_x;
	int threads_y = thread_cfg.threads_y;
	// The threads_x and threads_y are ignored in the case of SERIAL mesh type
	if (mesh_type_ == Mesh_type::SERIAL) {
		if (threads_x != 1 || threads_y != 1) {
			/*std::cout <<
				"Warning: ignoring the threads_x and threads_y parameters for the SERIAL mesh type!" <<
				std::endl;
				*/
			/* */
			threads_x = threads_y = 1;
		}
	}


	x_ranges = std::make_unique<std::array<int, 2>[]>(threads_x);
	y_ranges = std::make_unique<std::array<int, 2>[]>(threads_y);

	array_alloc_ptr = std::make_unique<array_allocator>(alloc_mode, alloc_major, threads_x * resolution[0], threads_y * resolution[1], _file_name);


#pragma omp parallel 
	{
		int num_threads = omp_get_num_threads();
		if (num_threads != threads_x * threads_y)
		{
			int thread_id = omp_get_thread_num();
			/*if (thread_id == 0)
				std::cout << "Warning: resetting the number of threads" << std::endl;
				*/
			/**/
			/**/
		}

	}

	omp_set_num_threads(threads_x * threads_y);

	int x_per_thread = (this->resolution[0] + threads_x - 1) / threads_x;
	for (int i = 0; i < threads_x; i++) {
		int x_first = i * this->resolution[0] + i * x_per_thread;
		int x_last = x_first + x_per_thread ;
		if (x_first >= this->resolution[0] * threads_x)
			x_first = this->resolution[0] * threads_x;
		if (x_last >= this->resolution[0] * threads_x)
			x_last = this->resolution[0] * threads_x ;
		x_ranges[i] = std::array<int, 2>{ x_first,x_last };
		/*/*/
	}


	int y_per_thread = (this->resolution[1] + threads_y - 1) / threads_y;
	for (int j = 0; j < threads_y; j++) {
		int y_first = j * this->resolution[1] + j * y_per_thread;
		int y_last = y_first + y_per_thread;
		if (y_first >= this->resolution[1] * threads_y)
			y_first = this->resolution[1] * threads_y;
		if (y_last >= this->resolution[1] * threads_y)
			y_last = this->resolution[1] * threads_y ;
		y_ranges[j] = std::array<int, 2>{ y_first,y_last };
	}

	for (int i = 0; i < threads_x; i++) {
		std::cout << x_ranges[i][0] - this->resolution[0] << "," << x_ranges[i][1] - this->resolution[0]  << std::endl;
	}

	std::cout << std::endl;
	for (int i = 0; i < threads_y; i++) {
		std::cout << y_ranges[i][0] - this->resolution[1] << "," << y_ranges[i][1] - this->resolution[1] << std::endl;
	}


	// The compiler complains about x_per_thread and y_per_thread
	// variables not used while they have been used.
	// To stop this complain we added this part
	x_per_thread = y_per_thread = 0;


	switch (mesh_type_) {
	case Mesh_type::OUTER_LOOP:
		run_strategy = std::make_unique<omp_strategy_i>(*this);
		break;
	case Mesh_type::INNER_LOOP:
		run_strategy = std::make_unique<omp_strategy_j>(*this);
		break;
	case Mesh_type::SERIAL:
		run_strategy = std::make_unique<omp_strategy_serial>(*this);
		break;

	}

	if (!burning_)
		frmla = std::make_unique<formula_1<double>>();
	else
		frmla = std::make_unique<formula_2<double>>();

}

void mandelbrot_omp::calculate(const double& scale_)
{
	run_strategy->calculate(scale_);
}