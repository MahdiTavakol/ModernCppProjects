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


enum class Animate_type { ANIMATE_1, ANIMATE_2, ANIMATE_3, ANIMATE_4, ANIMATE_5 };

namespace Runner_NS {
	class run_mandelbrot_animation : public run_mandelbrot
	{
	public:
		run_mandelbrot_animation(std::string info_,
			                     const bounds& bnds_,
			                     int x_size_, int y_size_,
			                     const thread_config& thread_cfg_,
			                     Mesh_type mesh_type_,
			                     Animate_type ani_type_ = Animate_type::ANIMATE_1,
								 allocation_mode alloc_mode_ = allocation_mode::CPP,
								 allocation_major alloc_major_ = allocation_major::X_MAJOR);

		~run_mandelbrot_animation() = default;

		void run() override;
	
	private:
		Animate_type ani_type;
		void generate_animation(const complex<double>& _center, int frame_init = 0, int num_frames = 100);
		void animate(std::string _file_name, const complex<double>& _center, const double& _scale);
	};
}