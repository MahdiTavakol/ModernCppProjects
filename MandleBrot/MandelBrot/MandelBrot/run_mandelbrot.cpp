#include "run_mandelbrot.h"

run_mandelbrot::run()
{
	std::cout << "Allocating the " << info << " mandlebrot pointer" << std::endl;
	std::string file_name("Mandelbrot_" + _info + ".dat");

	switch (mesh_type)
	{
	case mesh_type::INNER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot>(_allocation_mode, _bnds, _x_size, _y_size, _thread_config, file_name);
	}

std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
std::cout << "Starting the " << _info << " code" << std::endl;
mndlbrt->calculate();
std::cout << "Finished the " << _info << " code" << std::endl;
area = mndlbrt->return_area();
std::cout << "The area of the set is " << area << std::endl;

std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
std::chrono::duration<double, std::milli> timeRequired = (end - start);
//Let's get milliseconds out of that, and report
std::cout << "The " << _info << " code took " << timeRequired.count() << " milliseconds." << std::endl;



std::cout << "Writting the output " << std::endl;
mndlbrt->output();
std::cout << "Finished writing the output" << std::endl;

std::cout << "Deallocating the " << _info << " mandlebrot pointer" << std::endl;
delete mndlbrt;
mndlbrt = nullptr;
}