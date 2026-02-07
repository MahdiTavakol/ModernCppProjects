#include "run_mandelbrot_single.h"
#include "../Algorithm/mandelbrot_omp.h"

using namespace Runner_NS;

run_mandelbrot_single::run_mandelbrot_single(const std::vector<std::string>& args_) : 
	run_mandelbrot{ args_ }
{
	build_mandelbrot_ptr();
}

run_mandelbrot_single::run_mandelbrot_single(const allocation_mode& alloc_mode_, const allocation_major& alloc_major_,
	const bounds& bnds_, const thread_config& thread_cfg_, const std::string& info_, const Mesh_type& mesh_type_):
	     run_mandelbrot{alloc_mode_,alloc_major_,bnds_,thread_cfg_, info_, mesh_type_} 
{
	build_mandelbrot_ptr();
}

void run_mandelbrot_single::build_mandelbrot_ptr()
{
	mandelbrot_ptr = std::make_unique<mandelbrot_omp>(
		alloc_mode, alloc_major,
		bnds, x_size, y_size, thread_cfg, mesh_type,
		info,
		10000, 1.0, burning);
}

void run_mandelbrot_single::run()
{
	mandelbrot_ptr->calculate();
}