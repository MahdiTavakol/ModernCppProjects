#include "run_mandelbrot_factory.h"

#include "run_mandelbrot_animation.h"
#include "run_mandelbrot_timing.h"
#include "run_mandelbrot_single.h"

using namespace Runner_NS;



run_mandelbrot_factory::run_mandelbrot_factory(const std::vector<std::string>& args_)
{
	int argc = static_cast<int>(args_.size());
	int iarg = 1;

	if (argc < 2)
		throw std::invalid_argument("Invalid input arguments");

	run_type = args_[1];

	if (run_type == "single") {
		runner_ptr = std::make_unique<run_mandelbrot_single>(args_);
	}
	else if (run_type == "timing") {
		runner_ptr = std::make_unique<run_mandelbrot_timing>(args_);
	}
	else if (run_type == "animation") {
		runner_ptr = std::make_unique<run_mandelbrot_animation>(args_);
	}

}

void run_mandelbrot_factory::return_runner(std::unique_ptr<run_mandelbrot>&& run_mandelbrot_ptr) {
	// Sanity check
	if (runner_ptr == nullptr)
		throw std::runtime_error("The runner_ptr has not been initialized yet!");
	run_mandelbrot_ptr = std::move(runner_ptr);
}