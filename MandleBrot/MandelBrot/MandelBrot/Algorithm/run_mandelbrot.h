#pragma once
#include <map>

#include "../Array/array.h"
#include "../Array/array_allocator.h"
#include "mandelbrot.h"
#include "mandelbrot_xmesh_innerloop.h"
#include "mandelbrot_xmesh_outerloop.h"
#include "mandelbrot_ymesh_innerloop.h"
#include "mandelbrot_ymesh_outerloop.h"
#include "../definitions.h"

enum class Run_type {TIMING,ANIMATE};
enum class Animate_type {ANIMATE_1, ANIMATE_2, ANIMATE_3, ANIMATE_4, ANIMATE_5 };
enum class Mesh_type { SERIAL, XMESH_INNER_LOOP, XMESH_OUTER_LOOP, YMESH_INNER_LOOP, YMESH_OUTER_LOOP };

using Numerical_NS::complex;

template<Run_type run_type>
class run_mandelbrot
{
public:
	// ANIMATE-only ctor
	run_mandelbrot(std::string info_,
		           const bounds& bnds_,
		           int x_size_, int y_size_,
		           const thread_config& thread_cfg_,
		           Mesh_type mesh_type_,
		           Animate_type ani_type_,
		           allocation_mode alloc_mode_ = allocation_mode::MDSPAN,
		           allocation_major alloc_major_ = allocation_major::X_MAJOR)
		requires (run_type == Run_type::ANIMATE):
		           info{ std::move(info_) },
		           mesh_type{ mesh_type_ },
		           alloc_mode{ alloc_mode_ },
		           alloc_major{ alloc_major_ },
		           ani_type{ani_type_},
		           bnds{ bnds_ },
		           x_size{ x_size_ }, y_size{ y_size_ },
		           thread_cfg{ thread_cfg_ }
	{
		const double dx = bnds.x_max - bnds.x_min;
		const double dy = bnds.y_max - bnds.y_min;
		array_allocator(alloc_mode_, alloc_major_, dx, dy, "test.dat");
	}

	// TIMING-only ctor
	run_mandelbrot(const bounds& bnds_,
				   int x_size_, int y_size_)
		requires (run_type == Run_type::TIMING) :
		          bnds{ bnds_ }, 
		          x_size{ x_size_ }, y_size{ y_size_ } {}

	void run()
	{
		complex center(-0.743643887037151, 0.131825904205330);

		if (run_type == Run_type::TIMING)
		{
			generate_timing_info();
			return;
		}
		else if (run_type == Run_type::ANIMATE)
		{
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
				center = complex(-1.4011551890, 0);
				break;

			case Animate_type::ANIMATE_5:
				center = complex(-0.39054, 0.58679);
				break;
			default:
				throw std::invalid_argument("Wrong anitype");
			}
			generate_animation(center);
		}
	}

private:
	std::string info;
	allocation_mode alloc_mode;
	allocation_major alloc_major;
	Animate_type ani_type requires (run_type == Run_type::ANIMATE);
	Mesh_type mesh_type;
	bounds bnds;
	int x_size;
	int y_size;
	thread_config thread_cfg;
	Run_type run_type;
	double area;
	std::unique_ptr<mandelbrot> mandelbrot_ptr;

	std::map<std::string, double> timings;
	std::map<std::string, double> areas;


	void run_timing();
	void generate_animation(const complex<double>& _center, int frame_init = 0, int num_frames = 100);
	void generate_timing_info();
	void animate(std::string _file_name, const complex<double>& _center, const double& _scale);
	static void writeMaps(std::string _info_file_name, 
						  std::map<std::string, double>& _timings,
		                  std::map<std::string, double>& _areas);
};