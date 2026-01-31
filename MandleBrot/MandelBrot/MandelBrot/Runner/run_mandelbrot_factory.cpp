#include "run_mandelbrot_factory.h"

using namespace Runner_NS;

run_mandelbrot_factory::run_mandelbrot_factory(std::vector<std::string> args_)
{
	int argc = static_cast<int>(args_.size());
	int iarg = 1;

	int type;
	bounds bnds;
	std::array<int, 2> resolution;
	bool shouldIrender;
	double gamma;

	thread_config thread_cfg;
	std::string mesh_type_string, alloc_mode_string, alloc_major_string;

	auto invalid_arg_check = [](const int& iarg, const int& expectarg, const int& argc) {
		if (iarg + expectarg >= argc)
			throw std::invalid_argument("Invalid input arguments");
		};

	while (iarg < argc) {
		if (args_[iarg] == "--timing") {
			iarg++;
			continue;
		}
		else if (args_[iarg] == "--animation") {
			invalid_arg_check(iarg, 2, argc);
			if (args_[iarg + 1] == "true")
				shouldIrender = true;
			else
				shouldIrender = false;
			gamma = std::stod(args_[iarg + 2]);
			iarg+=3;
		}
		else if (args_[iarg] == "--type") {
			invalid_arg_check(iarg, 1, argc);
			type = std::stoi(args_[iarg + 1]);
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
	}
}