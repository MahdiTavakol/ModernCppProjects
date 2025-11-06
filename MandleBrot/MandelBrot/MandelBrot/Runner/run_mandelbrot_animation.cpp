#include "run_mandelbrot_animation.h"
#include <cstdio>
#include <thread>
#include <vector>


using namespace Runner_NS;

run_mandelbrot_animation::run_mandelbrot_animation(const Animate_type& ani_type_,
	bool shouldIRender_, const double& gamma_ ):
	        run_mandelbrot(),
	        ani_type{ ani_type_ },
			shouldIRender{shouldIRender_},
			gamma{gamma_}
            {}

void run_mandelbrot_animation::run()
{
	complex center(-0.743643887037151, 0.131825904205330);
	double decay_rate = 1.05;

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

	case Animate_type::BURNING_1:
		center = complex(-0.8, -0.03);
		decay_rate = 0.99999;
		break;

	case Animate_type::BURNING_2:
		center = complex(-1.75, -0.03);
		break;

	default:
		throw std::invalid_argument("Wrong anitype");
	}
	generate_animation(center,0,1000,decay_rate);
}


void run_mandelbrot_animation::generate_animation(const complex<double>& _center, int frame_init,
	int num_frames, double decay_rate) {
	double S0 = 1.0;
	double S = S0;

	constexpr int renderingNEvery = 50;
	constexpr int maxThreads = 8;
	int nThreads = std::min((num_frames + renderingNEvery - 1) / renderingNEvery,maxThreads);
	std::vector<std::thread> thrds;
	thrds.reserve(nThreads);
	int first_frame = 0;
	int last_frame;
	std::array<std::string, 2> tmpl = { "frame",".dat" };

	auto async_render = [&]()
		{
			int availThreads = nThreads - thrds.size();
			if (availThreads > 0) {
				if (first_frame >= num_frames) return;
				last_frame = first_frame + renderingNEvery - 1;
				last_frame = last_frame < num_frames-1 ? last_frame : num_frames-1;
				thrds.push_back(std::thread(svgrender,
					first_frame, 1, last_frame, size, tmpl));
				first_frame = last_frame + 1;
			}
			else {
				for (auto& thrd : thrds)
					thrd.join();
				thrds.clear();
			}
		};

	for (int i = 0; i < num_frames; i++) {
		S *= decay_rate;
		if (i < frame_init) continue;
		double zoom = S;
		std::string file_name("temp/frame-" + std::to_string(i) + ".dat");
		animate(file_name, _center, zoom);

		if (shouldIRender && renderingNEvery) {
			if (i && i % renderingNEvery == 0) {
				async_render();
			}
		}
	}

	if (shouldIRender && renderingNEvery) {
		// it is possible that not all the frames have been rendered by now
		while (first_frame < num_frames) {
			async_render();
		}
	}
	for (auto& thrd : thrds)
		thrd.join();

}

void run_mandelbrot_animation::animate(std::string _file_name, const complex<double>& _center, const double& _scale)
{
			bounds bnds;
			bnds.x_min = _center.real - (2.665 / _scale);
			bnds.x_max = _center.real + (2.665 / _scale);
			bnds.y_min = _center.imag - (1.5 / _scale);
			bnds.y_max = _center.imag + (1.5 / _scale);


			int x_size = size[0];
			int y_size = size[1];

			allocation_mode alloc_mode = allocation_mode::MODERN;
			allocation_major alloc_major = allocation_major::X_MAJOR;

			int num_threads;
#pragma omp parallel
			{
				num_threads = omp_get_num_threads();
			}

			thread_config  trd_cnfg_y_meshes;
			trd_cnfg_y_meshes.threads_x = num_threads;
			trd_cnfg_y_meshes.threads_y = 1;

			std::unique_ptr<mandelbrot> mandelbrot_ptr;
			if (ani_type != Animate_type::BURNING_1 && ani_type != Animate_type::BURNING_2)
				mandelbrot_ptr = std::make_unique<mandelbrot_xmesh_outerloop>
				(alloc_mode, alloc_major, bnds, x_size, y_size, trd_cnfg_y_meshes, _file_name,0.8);
			else if (ani_type == Animate_type::BURNING_1 || ani_type == Animate_type::BURNING_2)
				mandelbrot_ptr = std::make_unique<burningship_xmesh_outerloop>
				(alloc_mode, alloc_major, bnds, x_size, y_size, trd_cnfg_y_meshes, _file_name,0.8);


			std::cout << "Running mandelbrot file " << _file_name << std::endl;
			mandelbrot_ptr->calculate();
			std::cout << "Finished running mandelbrot file " << _file_name << std::endl;
			mandelbrot_ptr->output();
}

void run_mandelbrot_animation::svgrender(const int first_frame_, const int stride_, const int last_frame_,
	const std::array<int, 2> size,
	std::array<std::string, 2> templates)
{
	std::string command("Plotting/Mandelbrot_plot_frames.py temp/" + templates[0] + " ");
	command += std::to_string(first_frame_) + " ";
	command += std::to_string(stride_) + " ";
	command += std::to_string(last_frame_) + " ";
	command += std::to_string(size[0]) + " ";
	command += std::to_string(size[1]);

#ifdef _WIN32
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(),"r"), _pclose);
#else
	std::unique_ptr< FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
#endif
	if (!pipe) std::cerr <<"Warning: Cannot run the plotting tool";

	char buf[256];
	while (fgets(buf, sizeof(buf), pipe.get())) {
		/*discard everything*/
	}
}
