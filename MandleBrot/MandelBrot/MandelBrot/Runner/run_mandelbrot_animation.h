#pragma once
#include <chrono>
#include <string>
#include <array>
#include <string>
#include "run_mandelbrot.h"



namespace Runner_NS {
	class run_mandelbrot_animation : public run_mandelbrot
	{
	public:
		run_mandelbrot_animation(const std::vector<std::string>& args_);
		~run_mandelbrot_animation() = default;

		void run() override;
	
	private:
		double decay_rate;
		double scale0;


		void generate_animation(const complex<double>& _center, int frame_init = 0, 
			int num_frames = 10000, double decay_rate = 1.05, double scale0=1.0);
		void animate(std::string _file_name, const complex<double>& _center, const double& _scale);
		static void svgrender(const int first_frame_, const int stride_, const int last_frame_,
			const std::array<int, 2> size,
			std::array<std::string, 2> templates);

		std::array<int, 2> size{ 1920,1080 };
	};
}
