#include <iostream>
#include <memory>

#include "Runner/run_mandelbrot.h"
#include "Runner/run_mandelbrot_factory.h"
#include "cmd_args.h"
using Runner_NS::run_mandelbrot_factory;
using Runner_NS::run_mandelbrot;


int main(int argc, char** argv)
{
	try {
		std::vector<std::string> args;
		if (argc == 1) {
			std::cout << "Using the default parameters!" << std::endl;
			default_args(argv[0], args);
		}
		else if (argc == 2 && std::string(argv[1]) == "--help") {
			print_help(argv[0]);
		}
		else {
			args = args_vec(argc, argv);
		}
		auto factory = std::make_unique<run_mandelbrot_factory>(args);
		std::unique_ptr<run_mandelbrot> run_mandelbrot_ptr;
		factory->return_runner(run_mandelbrot_ptr);
		run_mandelbrot_ptr->run();
	}
	catch (std::bad_alloc &ae) {
		std::cerr << "C++ allocation failed:" << ae.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception& ae) {
		std::cerr << "Exception: " << ae.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}







