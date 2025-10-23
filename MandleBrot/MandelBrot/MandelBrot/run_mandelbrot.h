#pragma once
#include <map>

#include "array.h"
#include "array_allocator.h"
#include "mandelbrot.h"
#include "mandelbrot_xmesh_innerloop.h"
#include "mandelbrot_xmesh_outerloop.h"
#include "mandelbrot_ymesh_innerloop.h"
#include "mandelbrot_ymesh_outerloop.h"
#include "definitions.h"

enum class Run_type { TIMING, ANIMATE_1, ANIMATE_2, ANIMATE_3, ANIMATE_4, ANIMATE_5 };
enum class Mesh_type { SERIAL, XMESH_INNER_LOOP, XMESH_OUTER_LOOP, YMESH_INNER_LOOP, YMESH_OUTER_LOOP };

class run_mandelbrot
{
public:
	run_mandelbrot(std::string _info, const Mesh_type& mesh_type, const Run_type& run_type, 
		const allocation_mode alloc_mode_, const allocation_major alloc_major_, 
		const bounds& _bnds,
		const int& _x_size, const int& _y_size, const thread_config& _thread_config, double& area);
	run_mandelbrot(const bounds& _bnds);

	void run();

private:
	std::string info;
	allocation_mode alloc_mode;
	allocation_major alloc_major;
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
	double lastTiming{ 0.0 };


	void run_timing();
	void generate_timing_info();
	void generate_animation(const complex& _center, int frame_init = 0, int num_frames = 100);
	void animate(std::string _file_name, const complex& _center, const double& _scale);
	void writeMaps(std::string _info_file_name, std::map<std::string, double>& _timings, std::map<std::string, double>& _areas);

};