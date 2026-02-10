#include "run_mandelbrot.h"

#include <algorithm>
#include <omp.h>

using namespace Runner_NS;

run_mandelbrot::run_mandelbrot(const std::vector<std::string>& args_)
{
	int iarg = 2;
	int argc = static_cast<int>(args_.size());
	bool threadsSet = false;


	/* list of supported arguments
	 * information string --info info_string
	 * run types:
	 *	timing --timing
	 *  animation --animation
	 * type --> which animation type to run (based on the center)
	 * bounds --> mandelbrot bounds --bounds x_min x_max y_min y_max
	 * resolution --> mandelbrot resolution --resolution x_res y_res
	 * thread_config --> thread configuration --threads threads_x threads_y
	 * mesh_type --> mesh type --mesh_type mesh_type_string
	 * alloc_mode --> allocation mode --alloc_mod alloc_mode_string
	 * alloc_major --> allocation major --alloc_major alloc_major_string
	 */



	while (iarg < argc) {
		if (args_[iarg] == "--info") {
			invalid_arg_check(iarg, 1, argc);
			info = args_[iarg + 1];
			iarg += 2;
		}
		else if (args_[iarg] == "--type") {
			invalid_arg_check(iarg, 1, argc);
			int center_type_id = std::stoi(args_[iarg + 1]);
			parse_center(center_type_id, center, burning);
			bnds = center_scale_2_bounds(center);
			iarg += 2;
		}
		else if (args_[iarg] == "--bounds") {
			invalid_arg_check(iarg, 4, argc);
			bnds.x_min = std::stod(args_[iarg + 1]);
			bnds.x_max = std::stod(args_[iarg + 2]);
			bnds.y_min = std::stod(args_[iarg + 3]);
			bnds.y_max = std::stod(args_[iarg + 4]);
			iarg += 5;
		}
		else if (args_[iarg] == "--resolution") {
			invalid_arg_check(iarg, 2, argc);
			resolution[0] = std::stoi(args_[iarg + 1]);
			resolution[1] = std::stoi(args_[iarg + 2]);
			iarg += 3;
		}
		else if (args_[iarg] == "--threads") {
			invalid_arg_check(iarg, 2, argc);
			thread_cfg.threads_x = std::stoi(args_[iarg + 1]);
			thread_cfg.threads_y = std::stoi(args_[iarg + 2]);
			threadsSet = true;
			iarg += 3;
		}
		else if (args_[iarg] == "--num_iterations") {
			invalid_arg_check(iarg, 1, argc);
			num_iterations = std::stoi(args_[iarg + 1]);
			iarg += 2;
		}
		else if (args_[iarg] == "--mesh_type") {
			invalid_arg_check(iarg, 1, argc);
			std::string mesh_type_string = args_[iarg + 1];
			std::transform(mesh_type_string.begin(), mesh_type_string.end(), mesh_type_string.begin(), ::toupper);
			parse_mesh_type(mesh_type_string, mesh_type);
			iarg += 2;
		}
		else if (args_[iarg] == "--alloc_mod") {
			invalid_arg_check(iarg, 1, argc);
			std::string alloc_mode_string = args_[iarg + 1];
			std::transform(alloc_mode_string.begin(), alloc_mode_string.end(), alloc_mode_string.begin(), ::toupper);
			parse_allocation_mode(alloc_mode_string, alloc_mode);
			iarg += 2;
		}
		else if (args_[iarg] == "--alloc_major") {
			invalid_arg_check(iarg, 1, argc);
			std::string alloc_major_string = args_[iarg + 1];
			std::transform(alloc_major_string.begin(), alloc_major_string.end(), alloc_major_string.begin(), ::toupper);
			parse_allocation_major(alloc_major_string, alloc_major);
			iarg += 2;
		}
		else if (args_[iarg] == "--gamma") {
			invalid_arg_check(iarg, 1, argc);
			gamma = std::stoi(args_[iarg + 1]);
			iarg += 2;
		}
		else if (args_[iarg] == "--no_rendering") {
			shouldIRender = false;
			iarg++;
		}
		else if (args_[iarg] == "--num_frames") {
			invalid_arg_check(iarg, 1, argc);
			iarg += 2;
		}
		else if (args_[iarg] == "--noWrite") {
			// this is parsed again by the run_mandelbrot_timing 
			iarg++;
		}
		else
			throw std::invalid_argument("Unknown argument!");
	}

	if (!threadsSet) {
#pragma omp parallel
		{
			int num_threads = omp_get_num_threads();
			thread_cfg.threads_x = num_threads;
			thread_cfg.threads_y = 1;
		}
	}

}

run_mandelbrot::run_mandelbrot(const allocation_mode& alloc_mode_, const allocation_major& alloc_major_,
	const bounds& bnds_, const thread_config& thread_cfg_, const std::string& info_, const Mesh_type& mesh_type_):
	info{info_}, center{complex(-0.743643887037151, 0.131825904205330)}, bnds{ bnds_ },
	alloc_mode{alloc_mode_}, alloc_major{alloc_major_}, mesh_type{mesh_type_},
	resolution{1920,1080},
	thread_cfg{thread_cfg_}
{}

void run_mandelbrot::invalid_arg_check(const int& iarg, const int& expectarg, const int& argc) {
	if (iarg + expectarg >= argc)
		throw std::invalid_argument("Invalid input arguments");
}

void run_mandelbrot::parse_center(const int& center_type_id, complex<double>& center, bool& burning) {
	// Setting the center based on the center type
	switch (center_type_id)
	{
	case 1:
		center = complex(-0.743643887037151, 0.131825904205330);
		break;

	case 2:
		center = complex(-0.1015, 0.633);
		break;

	case 3:
		center = complex(-0.1015, -0.633);
		break;

	case 4:
		center = complex(-1.4011551890, 0.0);
		break;
	case 5:
		center = complex(-0.39054, 0.58679);
		break;
	case 6:
		center = complex(-0.761574, -0.0847596);
		break;
	case 7:
		center = complex(-0.7435669, 0.1314023);
		break;
	case 8:
		center = complex(-0.39054, -0.58679);
		break;
	case 9:
		center = complex(-1.401155, 0.0);
		break;

		// burning ship cases
	case 10:
		center = complex(-1.516, -0.03);
		burning = true;
		break;

	case 11:
		center = complex(-1.75, -0.03);
		burning = true;
		break;

	default:
		throw std::invalid_argument("Wrong center_type_id");
	}

}

void run_mandelbrot::parse_allocation_mode(const std::string& alloc_mode_string, allocation_mode& alloc_mode) {
	// Setting the alloc_mode_ based on the alloc_mode_string
	if (alloc_mode_string == "C")
	{
		alloc_mode = allocation_mode::C;
	}
	else if (alloc_mode_string == "CPP")
	{
		alloc_mode = allocation_mode::CPP;
	}
	else if (alloc_mode_string == "MODERN")
	{
		alloc_mode = allocation_mode::MODERN;
	}
	else if (alloc_mode_string == "MDSPAN")
	{
		alloc_mode = allocation_mode::MDSPAN;
	}
	else
		throw std::invalid_argument("Wrong allocation mode string");
}

void run_mandelbrot::parse_allocation_major(const std::string& alloc_major_string, allocation_major& alloc_major) {
	// Setting the alloc_major based on the alloc_major_string
	if (alloc_major_string == "X_MAJOR") {
		alloc_major = allocation_major::X_MAJOR;
	}
	else if (alloc_major_string == "Y_MAJOR") {
		alloc_major = allocation_major::Y_MAJOR;
	}
	else
		throw std::invalid_argument("Wrong allocation major string");
}

void run_mandelbrot::parse_mesh_type(const std::string& mesh_type_string, Mesh_type& mesh_type) {
	// Setting the mesh_type based on the mesh_type_string
	if (mesh_type_string == "INNER_LOOP") {
		mesh_type = Mesh_type::INNER_LOOP;
	}
	else if (mesh_type_string == "OUTER_LOOP")
	{
		mesh_type = Mesh_type::OUTER_LOOP;
	}
	else if (mesh_type_string == "SERIAL") {
		mesh_type = Mesh_type::SERIAL;
	}
	else
		throw std::invalid_argument("Wrong mesh_type_string");
}

bounds run_mandelbrot::center_scale_2_bounds(const complex<double>& _center, const double& _scale) {
	bounds bnds;
	bnds.x_min = _center.real - (2.665 / _scale);
	bnds.x_max = _center.real + (2.665 / _scale);
	bnds.y_min = _center.imag - (1.5 / _scale);
	bnds.y_max = _center.imag + (1.5 / _scale);
	return bnds;
}