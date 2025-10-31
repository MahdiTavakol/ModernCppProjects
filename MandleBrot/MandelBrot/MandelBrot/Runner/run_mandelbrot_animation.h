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
#include <array>
#include "run_mandelbrot.h"


enum class Animate_type { ANIMATE_1, ANIMATE_2, ANIMATE_3, ANIMATE_4, ANIMATE_5 };

namespace Runner_NS {
	class run_mandelbrot_animation : public run_mandelbrot
	{
	public:
		run_mandelbrot_animation(const Animate_type& ani_type_, bool shouldIRender = false);

		~run_mandelbrot_animation() = default;

		void run() override;
	
	private:
		Animate_type ani_type;
		bool shouldIRender;
		void generate_animation(const complex<double>& _center, int frame_init = 0, int num_frames = 10000);
		void animate(std::string _file_name, const complex<double>& _center, const double& _scale);
		static void svgrender(const int first_frame_, const int stride_, const int last_frame_,
			const std::array<int, 2> size,
			std::array<std::string, 2> templates);
	};
}