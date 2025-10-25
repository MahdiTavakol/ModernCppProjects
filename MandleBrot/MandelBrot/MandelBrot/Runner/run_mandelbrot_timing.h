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
	class run_mandelbrot_timing : public run_mandelbrot
	{
	public:
		run_mandelbrot_timing(const bounds& bnds_, int x_size_, int y_size_);

		void run() override;

	private:
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

		std::map<std::string, double> timings;
		std::map<std::string, double> areas;


		void run_timing();
		void generate_timing_info();

		static void writeMaps(std::string _info_file_name,
			                  std::map<std::string, double>& _timings,
			                  std::map<std::string, double>& _areas);
	};
}





