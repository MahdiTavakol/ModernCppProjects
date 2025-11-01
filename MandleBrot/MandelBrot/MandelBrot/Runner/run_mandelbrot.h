#pragma once
#include <map>
#include <chrono>
#include <optional>
#include <string>
#include <memory>
#include <stdexcept>
#include <utility>
#include <fstream>
#include <iostream>
#include <cstdlib>


#include "../Array/array.h"
#include "../Array/array_allocator.h"
#include "../Numerical/complex.h"
#include "../Algorithm/mandelbrot.h"
#include "../Algorithm/mandelbrot_xmesh_innerloop.h"
#include "../Algorithm/mandelbrot_xmesh_outerloop.h"
#include "../Algorithm/mandelbrot_ymesh_innerloop.h"
#include "../Algorithm/mandelbrot_ymesh_outerloop.h"
#include "../Algorithm/burningship_xmesh_outerloop.h"
#include "../definitions.h"

using Numerical_NS::complex;
using namespace Mandelbrot_NS;

namespace Runner_NS {
	enum class Mesh_type { SERIAL, XMESH_INNER_LOOP, XMESH_OUTER_LOOP, YMESH_INNER_LOOP, YMESH_OUTER_LOOP };

	class run_mandelbrot
	{
	public:
		run_mandelbrot(const bounds& bnds_,
			int x_size_, int y_size_) :
			bnds{ bnds_ }, x_size{ x_size_ }, y_size{ y_size_ } {}

		run_mandelbrot() {};

		run_mandelbrot(std::string info_,
			const bounds& bnds_,
			int x_size_, int y_size_,
			const thread_config& thread_cfg_,
			Mesh_type mesh_type_,
			allocation_mode alloc_mode_,
			allocation_major alloc_major_) :
				info{ std::move(info_) },
			    alloc_mode{ alloc_mode_ },
			    alloc_major{ alloc_major_ },
				mesh_type{ mesh_type_ },
				bnds{ bnds_ },
				x_size{ x_size_ }, y_size{ y_size_ },
				thread_cfg{ thread_cfg_ } {}

		virtual ~run_mandelbrot() = default;

		virtual void run() = 0;

	protected:
		std::string info;
		allocation_mode alloc_mode;
		allocation_major alloc_major;
		Mesh_type mesh_type;
		bounds bnds;
		int x_size;
		int y_size;
		thread_config thread_cfg;
		double area;
		std::unique_ptr<mandelbrot> mandelbrot_ptr;
	};
}




