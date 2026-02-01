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
		run_mandelbrot_timing(const std::string info_, const complex<double>& center_,
			                  const bounds& bnds_, const std::array<int, 2>& resolution_,
			                  const bool& shouldIRender_, const double& gamma_,
	                          const thread_config& thread_cfg,
			                  const std::string& mesh_type_string,
			                  const std::string& alloc_mode_string,
			                  const std::string& alloc_major_string);
		~run_mandelbrot_timing() = default;

		void run() override;

	private:
		int num_threads;
		std::map<std::string, double> timings;
		std::map<std::string, double> areas;


		void run_timing();
		void generate_timing_info();

		static void writeMaps(std::string _info_file_name,
			                  std::map<std::string, double>& _timings,
			                  std::map<std::string, double>& _areas);
	};
}





