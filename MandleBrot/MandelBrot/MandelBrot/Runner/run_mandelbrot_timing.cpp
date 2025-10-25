#include "run_mandelbrot_timing.h"

using namespace Runner_NS;

run_mandelbrot_timing::run_mandelbrot_timing(const bounds& bnds_, int x_size_, int y_size_) :
	bnds{ bnds_ }, x_size{ x_size_ }, y_size{ y_size_ } {}

void run_mandelbrot_timing::run()
{
	complex center(-0.743643887037151, 0.131825904205330);
	generate_timing_info();
	return;
}

void run_mandelbrot_timing::run_timing()
{
	std::cout << "Allocating the " << info << " mandelbrot pointer" << std::endl;
	std::string file_name("Mandelbrot_" + info + ".dat");
	std::string title;

	switch (mesh_type)
	{
	case Mesh_type::XMESH_INNER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot_xmesh_innerloop>(alloc_mode, alloc_major, bnds, x_size, y_size, thread_cfg, file_name);
		title = "X-Mesh Inner Loop";
		break;
	case Mesh_type::XMESH_OUTER_LOOP:
		title = "X-Mesh Outer Loop";
		mandelbrot_ptr = std::make_unique<mandelbrot_xmesh_outerloop>(alloc_mode, alloc_major, bnds, x_size, y_size, thread_cfg, file_name);
		break;
	case Mesh_type::YMESH_INNER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot_ymesh_innerloop>(alloc_mode, alloc_major, bnds, x_size, y_size, thread_cfg, file_name);
		title = "Y-Mesh Inner Loop";
		break;
	case Mesh_type::YMESH_OUTER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot_ymesh_outerloop>(alloc_mode, alloc_major, bnds, x_size, y_size, thread_cfg, file_name);
		title = "Y-Mesh Outer Loop";
		break;
	case Mesh_type::SERIAL:
		mandelbrot_ptr = std::make_unique<mandelbrot>(alloc_mode, alloc_major, bnds, x_size, y_size, file_name);
		title = "Serial";
		break;
	default:
		std::cerr << "Unknown mesh type!" << std::endl;
		exit(EXIT_FAILURE);
	}


	std::cout << "Starting the " << info << " code" << std::endl;
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	mandelbrot_ptr->calculate();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> timeRequired = (end - start);
	//Let's get milliseconds out of that, and report
	std::cout << "The " << info << " code took " << timeRequired.count() << " milliseconds." << std::endl;
	std::cout << "Finished the " << info << " code" << std::endl;
	area = mandelbrot_ptr->return_area();
	std::cout << "The area of the set is " << area << std::endl;

	std::cout << "Writting the output " << std::endl;
	mandelbrot_ptr->output();
	std::cout << "Finished writing the output" << std::endl;

	timings[info + " - " + title] = timeRequired.count();
	areas[info + " - " + title] = area;
}

void run_mandelbrot_timing::generate_timing_info()
{
	bounds bnds;
	bnds.x_min = -3.56;
	bnds.x_max = 1.77;
	bnds.y_min = -1.5;
	bnds.y_max = 1.5;
	int x_size = 1920;
	int y_size = 1080;


	int num_threads;
#pragma omp parallel
	{
		num_threads = omp_get_num_threads();
	}

	thread_config trd_cnfg_serial, trd_cnfg_x_meshes, trd_cnfg_y_meshes;

	trd_cnfg_x_meshes.threads_x = num_threads;
	trd_cnfg_x_meshes.threads_y = 1;
	trd_cnfg_y_meshes.threads_x = 1;
	trd_cnfg_y_meshes.threads_y = num_threads;


	auto run_diff_thread_configs = [&]()
		{
			mesh_type = Mesh_type::SERIAL;
			run_timing();

			thread_cfg = trd_cnfg_x_meshes;
			mesh_type = Mesh_type::XMESH_OUTER_LOOP;
			run_timing();

			mesh_type = Mesh_type::XMESH_INNER_LOOP;
			run_timing();

			thread_cfg = trd_cnfg_y_meshes;
			mesh_type = Mesh_type::YMESH_OUTER_LOOP;
			run_timing();

			mesh_type = Mesh_type::YMESH_INNER_LOOP;
			run_timing();
		};

	std::string timing_area_info_file("timing-area.csv");


	// C
	alloc_mode = allocation_mode::C;
	alloc_major = allocation_major::X_MAJOR;
	info = std::string("C_X_MAJOR");
	run_diff_thread_configs();
	alloc_major = allocation_major::Y_MAJOR;
	info = std::string("C_Y_MAJOR");
	run_diff_thread_configs();

	// CPP
	alloc_mode = allocation_mode::CPP;
	alloc_major = allocation_major::X_MAJOR;
	info = std::string("CPP_X_MAJOR");
	run_diff_thread_configs();
	alloc_major = allocation_major::Y_MAJOR;
	info = std::string("CPP_Y_MAJOR");
	run_diff_thread_configs();

	// modern
	alloc_mode = allocation_mode::MODERN;
	alloc_major = allocation_major::X_MAJOR;
	info = std::string("MODERN_X_MAJOR");
	run_diff_thread_configs();
	alloc_major = allocation_major::Y_MAJOR;
	info = std::string("MODERN_Y_MAJOR");
	run_diff_thread_configs();

	// mdspan
	alloc_mode = allocation_mode::MDSPAN;
	alloc_major = allocation_major::X_MAJOR;
	info = std::string("MDSPAN_X_MAJOR");
	run_diff_thread_configs();
	alloc_major = allocation_major::Y_MAJOR;
	info = std::string("MDSPAN_Y_MAJOR");
	run_diff_thread_configs();

	writeMaps(timing_area_info_file, timings, areas);

	std::cout << "All done!" << std::endl;

	return;
}


void run_mandelbrot_timing::writeMaps(std::string _info_file_name,
	                                  std::map<std::string, double>& _timings,
	                                  std::map<std::string, double>& _areas)
{
	std::cout << "Writing the timing info file " << std::endl;
	std::ofstream info_file(_info_file_name);

	if (!info_file.is_open())
		std::cerr << "Failed to open the " << _info_file_name << " for writing the timing information" << std::endl;

	info_file << "Run_type,area,timing(ms)" << std::endl;

	if (_timings.size() != _areas.size())
		std::cerr << "The size of the timing (" << _timings.size() << ") and area (" << _areas.size() << ") maps are different which is unexpected" << std::endl;

	auto it1 = _timings.begin();
	auto it2 = _areas.begin();
	for (; it1 != _timings.end() && it2 != _areas.end(); it1++, it2++)
		info_file << it1->first << "," << it2->second << "," << it1->second << std::endl;

	std::cout << "Closing the timing info file " << std::endl;

	info_file.close();
	std::cout << "Finished writing the timging info file " << std::endl;
}
