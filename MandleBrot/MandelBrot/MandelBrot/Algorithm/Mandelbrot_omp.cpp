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


	// calculating the number of deactivated threads
	auto deactive_threads = [&](int threads, int res) {
		int per_thread = (res + threads - 1) / threads;
		int total_data_expected = per_thread * threads;
		int extra_data = total_data_expected - res;
		int deactivated_threads = extra_data / per_thread;
		return deactivated_threads;
		};
	int deactivate_threads_x = deactive_threads(threads_x, this->resolution[0]);
	int deactivate_threads_y = deactive_threads(threads_y, this->resolution[1]);
	threads_x -= deactivate_threads_x;
	threads_y -= deactivate_threads_y;
	thread_cfg.threads_x = threads_x;
	thread_cfg.threads_y = threads_y;


	// Resetting the number of omp threads
	omp_set_num_threads(threads_x * threads_y);

	// Setting the data range for each thread in x dimension
	int x_per_thread = (resolution[0] + threads_x - 1) / threads_x;
	for (int i = 0; i < threads_x; i++) {
		int x_first = i * this->resolution[0] + i * x_per_thread;
		int x_last = x_first + x_per_thread ;
		if (i == threads_x - 1)
			x_last = this->resolution[0] * threads_x - 1;
		x_ranges[i] = std::array<int, 2>{ x_first,x_last };
		/*/*/
	}

	// Setting the data range for each thread in y dimension
	int y_per_thread = (resolution[0] + threads_y - 1) / threads_y;
	for (int j = 0; j < threads_y; j++) {
		int y_first = j * this->resolution[1] + j * y_per_thread;
		int y_last = y_first + y_per_thread;
		if (j == threads_y - 1)
			y_last = this->resolution[1] * threads_y - 1;
		y_ranges[j] = std::array<int, 2>{ y_first,y_last };
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