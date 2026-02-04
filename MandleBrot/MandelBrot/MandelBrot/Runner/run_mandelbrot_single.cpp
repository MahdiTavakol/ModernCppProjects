#include "run_mandelbrot_single.h"

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
	if (mesh_type != Mesh_type::SERIAL)
		mandelbrot_ptr = std::make_unique<mandelbrot_omp>(
			alloc_mode, alloc_major,
			bnds, x_size, y_size, thread_cfg, info,
			mesh_type,
			10000, 1.0, burning);
	else
		mandelbrot_ptr = std::make_unique<mandelbrot>(
			alloc_mode, alloc_major,
			bnds, x_size, y_size, thread_cfg, info,
			mesh_type,
			10000, 1.0, burning);

}

void run_mandelbrot_single::run()
{
	mandelbrot_ptr->calculate();
}