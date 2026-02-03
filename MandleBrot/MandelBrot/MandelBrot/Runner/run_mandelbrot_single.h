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


#include "run_mandelbrot.h"


using Numerical_NS::complex;
using namespace Mandelbrot_NS;

namespace Runner_NS {
	class run_mandelbrot_single : public run_mandelbrot
	{
	public:
		run_mandelbrot_single(const std::vector<std::string>& args_);
		run_mandelbrot_single(const allocation_mode& alloc_mode_, const allocation_major& alloc_major_,
			const bounds& bnds, const thread_config& thread_cfg, const std::string& info, const Mesh_type& mesh_type_);
		~run_mandelbrot_single() = default;

		void build_mandelbrot_ptr();

		void run() override;

	private:
	};
}
