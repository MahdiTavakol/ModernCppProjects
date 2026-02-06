#pragma once
#include <map>
#include <chrono>
#include <string>
#include <memory>


#include "run_mandelbrot.h"


using Numerical_NS::complex;
using namespace Mandelbrot_NS;

namespace Runner_NS {
	class run_mandelbrot_timing : public run_mandelbrot
	{
	public:
		run_mandelbrot_timing(const std::vector<std::string>& args_);
		~run_mandelbrot_timing() = default;

		void run() override;

	private:
		int num_threads;
		std::map<std::string, double> timings;
		std::map<std::string, double> areas;


		static void writeMaps(std::string _info_file_name,
			                  std::map<std::string, double>& _timings,
			                  std::map<std::string, double>& _areas);


		/// <summary>
		///  May be I need to replaced these with maps from allocation_mode to string to have the name of each one too. 
		/// </summary>
		std::vector<allocation_mode> alloc_mode_vec;
		std::vector<allocation_major> alloc_major_vec;
		std::vector<thread_config> threads_vec;
		std::vector<Mesh_type> mesh_vec;

		std::unique_ptr<run_mandelbrot> mandelbrot_runner_ptr;
	};
}





