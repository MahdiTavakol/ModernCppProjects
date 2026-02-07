#include "run_mandelbrot_timing.h"
#include "run_mandelbrot_single.h"
#include <omp.h>


using namespace Runner_NS;

run_mandelbrot_timing::run_mandelbrot_timing(const std::vector<std::string>& args_) :
	run_mandelbrot{ args_ }
{
#pragma omp parallel
	{
		num_threads = omp_get_num_threads();
	}

	// filling the allocation_mode vector
	alloc_mode_vec.push_back(allocation_mode::C);
	alloc_mode_vec.push_back(allocation_mode::CPP);
	alloc_mode_vec.push_back(allocation_mode::MODERN);
#if USE_MDSPAN
	alloc_mode_vec.push_back(allocation_mode::MDSPAN);
#endif


	// filling the alloc_major vector
	alloc_major_vec.push_back(allocation_major::X_MAJOR);
	alloc_major_vec.push_back(allocation_major::Y_MAJOR);

	
	// filling the thread config vector
	thread_config thread_cfg;
	thread_cfg.threads_x = num_threads;
	thread_cfg.threads_y = 1;
	threads_vec.push_back(thread_cfg);
	thread_cfg.threads_x = 1;
	thread_cfg.threads_y = num_threads;
	threads_vec.push_back(thread_cfg);

	// filling the mesh vector
	mesh_vec.push_back(Mesh_type::SERIAL);
	mesh_vec.push_back(Mesh_type::INNER_LOOP);
	mesh_vec.push_back(Mesh_type::OUTER_LOOP);

}


// May be instead of so many different functions 
// I can use strategy ... one strategy inheritance for
// the allocation mode 
// another for the allocation major and 
// finally one for the mesh type
void run_mandelbrot_timing::run()
{
	complex center(-0.743643887037151, 0.131825904205330);

	int number = 0;
	for (const auto& alloc_mode : alloc_mode_vec)
		for (const auto& alloc_major: alloc_major_vec)
			for (const auto& thread_cfg : threads_vec)
				for (const auto& mesh_type: mesh_vec)
				{
					number++;
					std::cout << "Allocating the #" << number << " mandelbrot pointer" << std::endl;
					std::string file_name("temp/Mandelbrot_" + std::to_string(number) + ".dat");
					std::string title;
					mandelbrot_runner_ptr = std::make_unique<run_mandelbrot_single>
						(alloc_mode, alloc_major, bnds, thread_cfg, file_name, mesh_type);


					std::cout << "Starting the #" << number << " code" << std::endl;
					std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
					mandelbrot_runner_ptr->run();

					auto end = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double, std::milli> timeRequired = (end - start);
					//Let's get milliseconds out of that, and report
					std::cout << "The #" << number << " code took " << timeRequired.count() << " milliseconds." << std::endl;
					std::cout << "Finished the " << info << " code" << std::endl;
					area = mandelbrot_runner_ptr->return_area();
					std::cout << "The area of the set is " << area << std::endl;

					std::cout << "Writting the output " << std::endl;
					mandelbrot_runner_ptr->output();
					std::cout << "Finished writing the output" << std::endl;

					timings[info + " - " + title] = timeRequired.count();
					areas[info + " - " + title] = area;
					
			    }


	std::string timing_area_info_file("timing-area.csv");


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
