#include "run_mandelbrot_animation.h"
#include "../Algorithm/mandelbrot_omp.h"
#include <cstdio>
#include <thread>
#include <vector>


using namespace Runner_NS;

constexpr int renderingNEvery = 50;
constexpr int maxRenderingThreads = 8;

run_mandelbrot_animation::run_mandelbrot_animation(const std::vector<std::string>& args_) :
	run_mandelbrot{ args_ }, decay_rate{ 1.05 }, scale0{ 1.0 }
{
	if (burning == true && std::abs(center.real +1.516) < 1e-6) {
		decay_rate = 0.95;
		scale0 = 10.0;
	}

	mandelbrot_ptr = std::make_unique<mandelbrot_omp>(
		alloc_mode, alloc_major,
		bnds, x_size, y_size, thread_cfg, mesh_type,
		info,
		num_iterations, gamma, burning);
}


void run_mandelbrot_animation::run()
{
	generate_animation(center,0,1000,decay_rate,scale0);
}


void run_mandelbrot_animation::generate_animation(const complex<double>& _center, int frame_init,
	int num_frames, double decay_rate, double scale0) {
	double S0 = scale0;
	double S = S0;

	// Rendering settings  --->>>
	int nRenderingThreads = std::min((num_frames + renderingNEvery - 1) / renderingNEvery,maxRenderingThreads);
	std::vector<std::thread> thrds;
	thrds.reserve(nRenderingThreads);
	int first_rendering_frame = 0;
	int last_rendering_frame;
	std::array<std::string, 2> tmpl = { info,".dat" };

	auto async_render = [&]()
		{
			int availThreads = nRenderingThreads - static_cast<int>(thrds.size());
			if (availThreads > 0) {
				if (first_rendering_frame >= num_frames) return;
				last_rendering_frame = first_rendering_frame + renderingNEvery - 1;
				last_rendering_frame = last_rendering_frame < num_frames-1 ? last_rendering_frame : num_frames-1;
				thrds.push_back(std::thread(svgrender,
					first_rendering_frame, 1, last_rendering_frame,
					std::array<int,2>{x_size,y_size}, tmpl));
				first_rendering_frame = last_rendering_frame + 1;
			}
			else {
				for (auto& thrd : thrds)
					thrd.join();
				thrds.clear();
			}
		};
		
	std::string command("mkdir temp");

#ifdef _WIN32
	std::unique_ptr<FILE,decltype(&_pclose)> pipe(_popen(command.c_str(),"r"),_pclose);
#else
	std::unique_ptr<FILE,decltype(&pclose)> pipe(popen(command.c_str(),"r"),pclose);
#endif

	char buf[256];
	while(fgets(buf,sizeof(buf),pipe.get())) {}

	// <<<--- rendering settings 

	// Going through the frames
	for (int i = 0; i < num_frames; i++) {
		S *= decay_rate;
		if (i < frame_init) continue;
		double zoom = S;
		std::string file_name("temp/" + tmpl[0] + "-" + std::to_string(i) + ".dat");

		// Calculation
		animate(file_name, _center, zoom);

		// Rendering 
		if (shouldIRender && renderingNEvery) {
			if (i && i % renderingNEvery == 0) {
				async_render();
			}
		}
	}

	if (shouldIRender && renderingNEvery) {
		// it is possible that not all the frames have been rendered by now
		while (first_rendering_frame < num_frames) {
			async_render();
		}
	}
	for (auto& thrd : thrds)
		thrd.join();

}

void run_mandelbrot_animation::animate(std::string _file_name, const complex<double>& _center, const double& _scale)
{
	using namespace Mandelbrot_NS;

	bounds bnds = center_scale_2_bounds(_center, _scale);


	std::unique_ptr<mandelbrot> mandelbrot_ptr;

	mandelbrot_ptr = std::make_unique<mandelbrot_omp>(
			alloc_mode, alloc_major,
			bnds, x_size, y_size, thread_cfg, mesh_type, 
			_file_name,
			num_iterations, gamma, burning);
			

	std::cout << "Running mandelbrot file " << _file_name << std::endl;
	mandelbrot_ptr->calculate();
	std::cout << "Finished running mandelbrot file " << _file_name << std::endl;
	mandelbrot_ptr->output();
	// putting the areas
	areas[_file_name] = mandelbrot_ptr->return_area();
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
	std::unique_ptr<FILE, decltype(&pclose)>  pipe(popen(command.c_str(), "r"), pclose);
#endif
	if (!pipe) std::cerr <<"Warning: Cannot run the plotting tool";

	char buf[256];
	while (fgets(buf, sizeof(buf), pipe.get())) {
		/*discard everything*/
	}
}

std::vector<int> run_mandelbrot_animation::return_area_vec()
{
	std::vector<int> out;
	for (auto it = areas.begin(); it != areas.end(); it++) {
		out.push_back(it->second);
	}
	return out;
}