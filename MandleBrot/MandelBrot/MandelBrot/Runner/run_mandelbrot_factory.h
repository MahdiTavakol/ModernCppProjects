#pragma once

#include "run_mandelbrot.h"
#include "run_mandelbrot_animation.h"
#include "run_mandelbrot_timing.h"

namespace Runner_NS {

	enum class Run_type 
	{ NONE, TIMING, ANIMATION };



	class run_mandelbrot_factory {
	public:
		run_mandelbrot_factory(std::vector<std::string> args_);
		~run_mandelbrot_factory() = default;

		void return_runner(std::unique_ptr<run_mandelbrot>&& run_mandelbrot_ptr);
	private:

		// run settings
		// info string
		std::string info;
		// which center
		int center_type_id;
		// center coordiation
		complex<double> center;
		// bounds
		bounds bnds;
		// resolution
		std::array<int, 2> resolution;
		// should I render (just for animation)
		bool shouldIrender;
		// gamma 
		double gamma;
		// thread configuration (just for animation)
		thread_config thread_cfg;
		std::string mesh_type_string, alloc_mode_string, alloc_major_string;

		Run_type run_type = Run_type::NONE;

		// the runner pointer
		std::unique_ptr<run_mandelbrot> runner_ptr;

	};
}