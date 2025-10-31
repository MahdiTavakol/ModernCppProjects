#include "run_mandelbrot_animation.h"


using namespace Runner_NS;

run_mandelbrot_animation::run_mandelbrot_animation(const Animate_type& ani_type_ ):
	        run_mandelbrot(),
	        ani_type{ ani_type_ }
            {}

void run_mandelbrot_animation::run()
{
	complex center(-0.743643887037151, 0.131825904205330);

	switch (ani_type)
	{
	case Animate_type::ANIMATE_1:
		center = complex(-0.743643887037151, 0.131825904205330);
		break;

	case Animate_type::ANIMATE_2:
		center = complex(-0.1015, 0.633);
		break;

	case Animate_type::ANIMATE_3:
		center = complex(-0.1015, 0.633);
		break;

	case Animate_type::ANIMATE_4:
		center = complex(-1.4011551890, 0.0);
		break;

	case Animate_type::ANIMATE_5:
		center = complex(-0.39054, 0.58679);
		break;
	default:
		throw std::invalid_argument("Wrong anitype");
	}
	generate_animation(center);
}


void run_mandelbrot_animation::generate_animation(const complex<double>& _center, int frame_init, int num_frames) {
	double S0 = 1.0;
	double decay_rate = 1.05;
	double S = S0;


	for (int i = 0; i < num_frames; i++) {
		S *= decay_rate;
		if (i < frame_init) continue;
		double zoom = S;
		std::string file_name("/temp/frame-" + std::to_string(i));
		animate(file_name, _center, zoom);
	}
}

void run_mandelbrot_animation::animate(std::string _file_name, const complex<double>& _center, const double& _scale)
{
			bounds bnds;
			bnds.x_min = _center.real - (2.665 / _scale);
			bnds.x_max = _center.real + (2.665 / _scale);
			bnds.y_min = _center.imag - (1.5 / _scale);
			bnds.y_max = _center.imag + (1.5 / _scale);


			int x_size = 1920;
			int y_size = 1080;

			allocation_mode alloc_mode = allocation_mode::MODERN;
			allocation_major alloc_major = allocation_major::X_MAJOR;

			int num_threads;
#pragma omp parallel
			{
				num_threads = omp_get_num_threads();
			}

			thread_config  trd_cnfg_y_meshes;
			trd_cnfg_y_meshes.threads_x = 1;
			trd_cnfg_y_meshes.threads_y = num_threads;

			std::unique_ptr<mandelbrot> mandelbrot_ptr;
			mandelbrot_ptr = std::make_unique<mandelbrot_xmesh_outerloop>(alloc_mode, alloc_major, bnds, x_size, y_size, trd_cnfg_y_meshes, _file_name);


			std::cout << "Running mandelbrot file " << _file_name << std::endl;
			mandelbrot_ptr->calculate();
			std::cout << "Finished running mandelbrot file " << _file_name << std::endl;
			mandelbrot_ptr->output();
}