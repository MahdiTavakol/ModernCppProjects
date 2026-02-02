#include "run_mandelbrot.h"

using namespace Runner_NS;

run_mandelbrot::run_mandelbrot(const std::vector<std::string>& args_)
{
	int iarg = 2;
	int argc = args_.size();

	auto invalid_arg_check = [](const int& iarg, const int& expectarg, const int& argc) {
		if (iarg + expectarg >= argc)
			throw std::invalid_argument("Invalid input arguments");
		};

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
	 * aloc_mode --> allocation mode --alloc_mod alloc_mode_string
	 * aloc_major --> allocation major --alloc_major alloc_major_string
	 */



	while (iarg < argc) {
		if (args_[iarg] == "--info") {
			invalid_arg_check(iarg, 1, argc);
			info = args_[iarg + 1];
			iarg += 2;
		}
		else if (args_[iarg] == "--type") {
			invalid_arg_check(iarg, 1, argc);
			center_type_id = std::stoi(args_[iarg + 1]);
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
			resolution[0] = std::stod(args_[iarg + 1]);
			resolution[1] = std::stod(args_[iarg + 2]);
			iarg += 3;
		}
		else if (args_[iarg] == "--threads") {
			invalid_arg_check(iarg, 2, argc);
			thread_cfg.threads_x = std::stoi(args_[iarg + 1]);
			thread_cfg.threads_y = std::stoi(args_[iarg + 2]);
			iarg += 3;
		}
		else if (args_[iarg] == "--mesh_type") {
			invalid_arg_check(iarg, 1, argc);
			mesh_type_string = args_[iarg + 1];
			iarg += 2;
		}
		else if (args_[iarg] == "--alloc_mod") {
			invalid_arg_check(iarg, 1, argc);
			alloc_mode_string = args_[iarg + 1];
			iarg += 2;
		}
		else if (args_[iarg] == "--alloc_major") {
			invalid_arg_check(iarg, 1, argc);
			alloc_major_string = args_[iarg + 1];
			iarg += 2;
		}
		else if (args_[iarg] == "--no_rendering") {
			shouldIRender = false;
			iarg++;
		}
	}

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
		center = complex(-0.1015, 0.633);
		break;

	case 4:
		center = complex(-1.4011551890, 0.0);
		break;
	case 5:
		center = complex(-0.39054, 0.58679);
		break;
	case6:
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
		break;

	case 11:
		center = complex(-1.75, -0.03);
		break;

	default:
		throw std::invalid_argument("Wrong anitype");
	}



}