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
#include <array>


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
	
	using Array_NS::allocation_mode;
	using Array_NS::allocation_major;

	class run_mandelbrot
	{
	public:
		run_mandelbrot(const std::vector<std::string>& args_);
		run_mandelbrot(const allocation_mode& alloc_mode_, const allocation_major& alloc_major_,
			const bounds& bnds, const thread_config& thread_cfg, const std::string& info, const Mesh_type& mesh_type_);


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

		double& return_area() {
			mandelbrot_ptr->return_area();
		}
		void output() {
			mandelbrot_ptr->output();
		}

	protected:
		// info string
		std::string info;
		// center point
		int center_type_id;
		complex<double> center;
		// bounds
		bounds bnds;
		// allocation mode
		std::string alloc_mode_string, alloc_major_string;
		allocation_mode alloc_mode;
		allocation_major alloc_major;
		// mesh type
		std::string mesh_type_string;
		Mesh_type mesh_type;
		// resolution
		std::array<int, 2> resolution;
		// should I render (just for animation)
		bool shouldIRender= true;
		// gamma
		double gamma;
		// thread configuration
		thread_config thread_cfg;
		int x_size;
		int y_size;
		double area;

		// the mandelbrot pointer
		std::unique_ptr<mandelbrot> mandelbrot_ptr;
	};
}




