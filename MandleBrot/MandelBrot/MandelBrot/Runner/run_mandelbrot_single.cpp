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
	switch (mesh_type)
	{
	case Mesh_type::XMESH_INNER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot_xmesh_innerloop>(alloc_mode, alloc_major, bnds, x_size, y_size, thread_cfg, info);
		info += "X-Mesh Inner Loop";
		break;
	case Mesh_type::XMESH_OUTER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot_xmesh_outerloop>(alloc_mode, alloc_major, bnds, x_size, y_size, thread_cfg, info);
		info += "X-Mesh Outer Loop";
		break;
	case Mesh_type::YMESH_INNER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot_ymesh_innerloop>(alloc_mode, alloc_major, bnds, x_size, y_size, thread_cfg, info);
		info += "Y-Mesh Inner Loop";
		break;
	case Mesh_type::YMESH_OUTER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot_ymesh_outerloop>(alloc_mode, alloc_major, bnds, x_size, y_size, thread_cfg, info);
		info += "Y-Mesh Outer Loop";
		break;
	case Mesh_type::SERIAL:
		mandelbrot_ptr = std::make_unique<mandelbrot>(alloc_mode, alloc_major, bnds, x_size, y_size, info);
		info += "Serial";
		break;
	default:
		std::cerr << "Unknown mesh type!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void run_mandelbrot_single::run()
{
	mandelbrot_ptr->calculate();
}