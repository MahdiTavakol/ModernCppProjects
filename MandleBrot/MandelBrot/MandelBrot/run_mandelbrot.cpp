#include "run_mandelbrot.h"
#include <chrono>

run_mandelbrot::run_mandelbrot(std::string _info, const Mesh_type& mesh_type_, const Run_type& run_type_,  const allocation_mode alloc_mode_, const bounds& bnds_,
		const int& x_size_, const int& y_size_, const thread_config& thread_config_, double& area_):
			info{_info}, mesh_type{mesh_type_}, run_type{run_type_}, alloc_mod{alloc_mode}, bnds{bnds_}, x_size{x_size_}, y_size{y_size_},
			thread_config{thread_config_}, area{area_}
{
	array_allocator(alloc_mode_, bnds_.x, bnds_.y, output_name_);
}

void run_mandelbrot::run()
{
	complex center(-0.743643887037151, 0.131825904205330);
	switch (run_type)
	{
		case Run_type::TIMING:
			run_timing();
			return;
			break;
		case Run_type::ANIMATE_1:
		{
			center = complex(-0.743643887037151, 0.131825904205330);
			break;
		}
		case Run_type::ANIMATE_2:
		{
			center = complex(-0.1015, 0.633);
			break;
		}
		case Run_type::ANIMATE_4:
		{
			center = complex(-1.4011551890, 0);
			break;
		}
		case Run_type::ANIMATE_5:
		{
			center = complex(-0.39054, 0.58679);
			break;
		}
		default:
			std::cerr << "Unknown run type!" << std::endl;
			exit(EXIT_FAILURE);
			break;
	}
	generate_animation(center);
}

void run_mandelbrot::run_timing()
{
	std::cout << "Allocating the " << info << " mandlebrot pointer" << std::endl;
	std::string file_name("Mandelbrot_" + info + ".dat");
	std::string title;

	switch (mesh_type)
	{
	case Mesh_type::XMESH_INNER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot_xmesh_innerloop>(alloc_mode, bnds, x_size, y_size, thread_cfg, file_name);
		title = "X-Mesh Inner Loop";
		break;
	case Mesh_type::XMESH_OUTER_LOOP:
		title = "X-Mesh Outer Loop";
		mandelbrot_ptr = std::make_unique<mandelbrot_xmesh_outerloop>(alloc_mode, bnds, x_size, y_size, thread_cfg, file_name);
		break;
	case Mesh_type::YMESH_INNER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot_ymesh_innerloop>(alloc_mode, bnds, x_size, y_size, thread_cfg, file_name);
		title = "Y-Mesh Inner Loop";
		break;
	case Mesh_type::YMESH_OUTER_LOOP:
		mandelbrot_ptr = std::make_unique<mandelbrot_ymesh_outerloop>(alloc_mode, bnds, x_size, y_size, thread_cfg, file_name);
		title = "Y-Mesh Outer Loop";
		break;
	case Mesh_type::SERIAL:
		mandelbrot_ptr = std::make_unique<mandelbrot>(alloc_mode, bnds, x_size, y_size, thread_cfg, file_name);
		title = "Serial";
		break;
	default:
		std::cerr << "Unknown mesh type!" << std::endl;
		exit(EXIT_FAILURE);
	}


	std::cout << "Starting the " << info << " code" << std::endl;
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	mandelbrot_ptr->calculate();
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> timeRequired = (end - start);
	//Let's get milliseconds out of that, and report
	std::cout << "The " << info << " code took " << timeRequired.count() << " milliseconds." << std::endl;
	std::cout << "Finished the " << info << " code" << std::endl;
	area = mandelbrot_ptr->return_area();
	std::cout << "The area of the set is " << area << std::endl;



	std::cout << "Writting the output " << std::endl;
	mandelbrot_ptr->output();
	std::cout << "Finished writing the output" << std::endl;

	//std::cout << "Deallocating the " << info << " mandlebrot pointer" << std::endl;

	timings[info + " - " + title] = timeRequired.count();
	areas[info + " - " + title] = area;

}


void run_mandelbrot::generate_animation(const complex& _center, int frame_init, int num_frames)
{
	double S0 = 1.0;
	double decay_rate = 1.05;
	double S = S0;


	for (int i = 0; i < num_frames; i++) {
		S *= decay_rate;
		if (i < frame_init) continue;
		double zoom = S;
		std::string file_name("frame-" + std::to_string(i));
		animate(file_name, _center, zoom);
	}
}


void run_mandelbrot::animate(std::string _file_name, const complex& _center, const double& _scale)
{
	bounds bnds;
	bnds.x_min = _center.real - (2.665 / _scale);
	bnds.x_max = _center.real + (2.665 / _scale);
	bnds.y_min = _center.imag - (1.5 / _scale);
	bnds.y_max = _center.imag + (1.5 / _scale);


	int x_size = 1920;
	int y_size = 1080;

	allocation_mode alloc_mode = allocation_mode::MODERN_C_X_MAJOR;

	double area;
	int num_threads;
#pragma omp parallel
	{
		num_threads = omp_get_num_threads();
	}

	thread_config  trd_cnfg_y_meshes;
	trd_cnfg_y_meshes.threads_x = 1;
	trd_cnfg_y_meshes.threads_y = num_threads;

	std::unique_ptr<mandelbrot> mandelbrot_ptr;
	mandelbrot_ptr = std::make_unique<mandelbrot_xmesh_outerloop>(alloc_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, _file_name);


	std::cout << "Running mandelbrot file " << _file_name << std::endl;
	mandelbrot_ptr->calculate();
	std::cout << "Finished running mandelbrot file " << _file_name << std::endl;
	mandelbrot_ptr->output();
}

void run_mandelbrot::writeMaps(std::string _info_file_name, std::map<std::string, double>& _timings, std::map<std::string, double>& _areas)
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

void run_mandelbrot::generate_timing_info()

{
	bounds bnds;
	bnds.x_min = -3.56;
	bnds.x_max = 1.77;
	bnds.y_min = -1.5;
	bnds.y_max = 1.5;
	int x_size = 1920;
	int y_size = 1080;

	int allocation_mode;

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


	std::string timing_area_info_file("timing-area.csv");



	mesh_type = Mesh_type::SERIAL;
	alloc_mode = allocation_mode::C_X_MAJOR;
	info = std::string("C_X_MAJOR");
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

	alloc_mode = allocation_mode::C_Y_MAJOR;
	info = std::string("C_Y_MAJOR");
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

	alloc_mode = allocation_mode::CPP_X_MAJOR;
	info = std::string("CPP_X_MAJOR");
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

	alloc_mode = allocation_mode::CPP_Y_MAJOR;
	info = std::string("CPP_Y_MAJOR");
	run_timing();

	thread_cfg = trd_cnfg_x_meshes;
	mesh_type = Mesh_type::XMESH_OUTER_LOOP;
	run_timing();

	mesh_type = Mesh_type::XMESH_INNER_LOOP;
	run_timing();

	allocation_mode = C_Y_MAJOR;
	info = "4-C_Y_MAJOR---X_MESHES";
	timing = run_mandelbrot<mandelbrot_xmesh>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);

	info = "5-C_Y_MAJOR---Y_MESHES";
	timing = run_mandelbrot<mandelbrot_ymesh>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);

	allocation_mode = CPP_X_MAJOR;
	info = "6-CPP_X_MAJOR---serial";
	timing = run_mandelbrot<mandelbrot>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_serial, area);

	info = "7-CPP_X_MAJOR---X_MESHES";
	timing = run_mandelbrot<mandelbrot_xmesh>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);

	info = "8-CPP_X_MAJOR---Y_MESHES";
	timing = run_mandelbrot<mandelbrot_ymesh>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);

	allocation_mode = CPP_Y_MAJOR;
	info = "9-CPP_Y_MAJOR---X_MESHES";
	timing = run_mandelbrot<mandelbrot_xmesh>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
;
	info = "10-CPP_Y_MAJOR---Y_MESHES";
	timing = run_mandelbrot<mandelbrot_ymesh>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);

	allocation_mode = C_X_MAJOR;
	info = "11-C_X_MAJOR---X_MESHES---OMP_INNER";
	timing = run_mandelbrot<mandelbrot_xmesh_innerloop>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);

	info = "12-C_X_MAJOR---Y_MESHES---OMP_INNER";
	timing = run_mandelbrot<mandelbrot_ymesh_innerloop>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);

	allocation_mode = C_Y_MAJOR;
	info = "13-C_Y_MAJOR---X_MESHES---OMP_INNER";
	timing = run_mandelbrot<mandelbrot_xmesh_innerloop>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "14-C_Y_MAJOR---Y_MESHES---OMP_INNER";
	timing = run_mandelbrot<mandelbrot_ymesh_innerloop>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	allocation_mode = CPP_X_MAJOR;
	info = "15-CPP_X_MAJOR---X_MESHES---OMP_INNER";
	timing = run_mandelbrot<mandelbrot_xmesh_innerloop>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "16-CPP_X_MAJOR---Y_MESHES---OMP_INNER";
	timing = run_mandelbrot<mandelbrot_ymesh_innerloop>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	allocation_mode = CPP_Y_MAJOR;
	info = "17-CPP_Y_MAJOR---X_MESHES---OMP_INNER";
	timing = run_mandelbrot<mandelbrot_xmesh_innerloop>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_x_meshes, area);
	timings[info] = timing;
	areas[info] = area;
	info = "18-CPP_Y_MAJOR---Y_MESHES---OMP_INNER";
	timing = run_mandelbrot<mandelbrot_ymesh_innerloop>(info, allocation_mode, bnds, x_size, y_size, trd_cnfg_y_meshes, area);
	timings[info] = timing;
	areas[info] = area;


	writeMaps(timing_area_info_file, timings, areas);

	std::cout << "All done!" << std::endl;

	return;
}
