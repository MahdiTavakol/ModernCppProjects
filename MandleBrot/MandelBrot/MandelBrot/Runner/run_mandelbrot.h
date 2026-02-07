#pragma once
#include <chrono>
#include <string>
#include <memory>
#include <utility>
#include <array>


#include "../Array/array.h"
#include "../Numerical/complex.h"
#include "../Algorithm/mandelbrot.h"
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


		//run_mandelbrot() {};


		virtual ~run_mandelbrot() = default;

		virtual void run() = 0;

		double& return_area() {
			return mandelbrot_ptr->return_area();
		}
		void output() {
			mandelbrot_ptr->output();
		}

	protected:
		// info string
		std::string info{ "test" };
		// center point
		complex<double> center{ -0.743643887037151, 0.131825904205330 };
		// bounds
		bounds bnds{ -3.56, 1.77, -1.50, 1.50 };
		// allocation mode
		allocation_mode alloc_mode{ allocation_mode::MODERN };
		allocation_major alloc_major{ allocation_major::X_MAJOR };
		// mesh type
		Mesh_type mesh_type{ Mesh_type::INNER_LOOP };
		// resolution
		std::array<int, 2> resolution{ 1920,1080 };
		// is it burning
		bool burning =  false;
		// number of iterations
		int num_iterations = 10000;
		// should I render (just for animation)
		bool shouldIRender{ true };
		// gamma
		double gamma{1.0};
		// thread configuration
		thread_config thread_cfg{};
		int x_size{ 0 };
		int y_size{ 0 };
		double area{ 0.0 };

		// the mandelbrot pointer
		std::unique_ptr<mandelbrot> mandelbrot_ptr;

	private:
		static void invalid_arg_check(const int& iarg, const int& expectarg, const int& argc);
		static void parse_center(const int& center_type_id, complex<double>& center, bool& burning);
		static void parse_allocation_mode(const std::string& alloc_mode_string, allocation_mode& alloc_mode);
		static void parse_allocation_major(const std::string& alloc_major_string, allocation_major& alloc_major);
		static void parse_mesh_type(const std::string& mesh_type_string, Mesh_type& mesh_type);

	};
}




