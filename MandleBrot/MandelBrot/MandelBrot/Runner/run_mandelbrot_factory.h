#pragma once

#include "run_mandelbrot.h"
#include "run_mandelbrot_animation.h"
#include "run_mandelbrot_timing.h"

namespace Runner_NS {
	class run_mandelbrot_factory {
	public:
		run_mandelbrot_factory(std::vector<std::string> args_);
		~run_mandelbrot_factory() = default;

		void return_runner(std::unique_ptr<run_mandelbrot>&& run_mandelbrot_ptr);
	private:
	};
}