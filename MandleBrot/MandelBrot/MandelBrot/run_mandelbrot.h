#pragma once

#include "array.h"
#include "array_allocator.h"
#include "mandelbrot.h"
#include "mandelbrot_xmesh_innerloop.h"
#include "mandelbrot_xmesh_outerloop.h"
#include "mandelbrot_ymesh_innerloop.h"
#include "mandelbrot_ymesh_outerloop.h"
#include "definitions.h"

enum class Run_type { TIMING, ANIMATE_1, ANIMATE_2, ANIMATE_3, ANIMATE_4, ANIMATE_5 };
enum class Mesh_type { SERIAL, INNER_LOOP, OUTER_LOOP };

class run_mandelbrot
{
public:
	run_mandelbrot(std::string _info, const Mesh_type& mesh_type, const Run_type& run_type,  const allocation_mode alloc_mode_, const bounds& _bnds,
		const int& _x_size, const int& _y_size, const thread_config& _thread_config, double& area);

	void run();

private:
	std::string info;
	allocation_mode alloc_mode;
	Mesh_type mesh_type;
	bounds bnds;
	int x_size;
	int y_size;
	thread_config thread_config;
	Run_type run_type;
	double area;
	std::unique_ptr<mandelbrot> mandelbrot_ptr;
};