#pragma once


#include <iostream>
#include <string>
#include <vector>


static void print_help(const char* exe)
{
    using std::cout;

    cout
        << "Mandelbrot renderer\n"
        << "\n"
        << "Usage:\n"
        << "  " << exe << " <mode> [options]\n"
        << "\n"
        << "Modes (first argument):\n"
        << "  animation   Render a sequence of frames (zoom / pan / etc.)\n"
        << "  single      Render a single image/frame\n"
        << "  timing      Run timing/benchmark mode (optionally disable rendering)\n"
        << "\n"
        << "Common options:\n"
        << "  --info <string>\n"
        << "      Free-form info tag used for labeling outputs/logs.\n"
        << "\n"
        << "  --type <int>\n"
        << "      Center/scene preset id (implementation-defined).\n"
        << "\n"
        << "  --bounds <x_min> <x_max> <y_min> <y_max>\n"
        << "      Complex-plane bounds to render.\n"
        << "      Example: --bounds -2.5 1.0 -1.2 1.2\n"
        << "\n"
        << "  --resolution <dx> <dy>\n"
        << "      Resolution step sizes (or pixel spacing), two doubles.\n"
        << "      Example: --resolution 0.002 0.002\n"
        << "\n"
        << "  --threads <tx> <ty>\n"
        << "      Thread tiling/config (two ints).\n"
        << "      Example: --threads 16 16\n"
        << "\n"
        << "  --mesh_type <type>\n"
        << "      Mesh type string (case-insensitive; will be uppercased internally).\n"
        << "      Example: --mesh_type CARTESIAN\n"
        << "\n"
        << "  --alloc_mod <string>\n"
        << "      Allocation mode selection (implementation-defined).\n"
        << "      Example: --alloc_mod malloc\n"
        << "\n"
        << "  --alloc_major <string>\n"
        << "      Memory layout major order selection (implementation-defined).\n"
        << "      Example: --alloc_major row\n"
        << "\n"
        << "  --no_rendering\n"
        << "      Disable rendering (useful for timing/benchmark runs).\n"
        << "\n"
        << "Examples:\n"
        << "  " << exe << " single --bounds -2.5 1.0 -1.2 1.2 --resolution 0.002 0.002 --threads 16 16\n"
        << "  " << exe << " timing --threads 32 8 --no_rendering\n"
        << "  " << exe << " animation --type 2 --mesh_type CARTESIAN --threads 16 16\n"
        << "\n"
        << "Notes:\n"
        << "  - <mode> must be the first argument.\n"
        << "  - All options are parsed after <mode> and may appear in any order.\n"
        << "  - Values for --type/--threads are integers; --bounds/--resolution are doubles.\n"
        << std::flush;
}

static void default_args(const std::string& progName, std::vector<std::string>& args_) {
    args_.reserve(10);

    args_.push_back(progName);
    args_.push_back("animation");

    args_.push_back("--info");
    args_.push_back("ani");

    args_.push_back("--type");
    args_.push_back(std::to_string(6));

    args_.push_back("--resolution");
    args_.push_back(std::to_string(1920));
    args_.push_back(std::to_string(1080));

    args_.push_back("--threads");
    args_.push_back(std::to_string(8));
    args_.push_back(std::to_string(1));

}

static std::vector<std::string> args_vec(int argc, char** argv) {
    std::vector<std::string> vec;
    for (int i = 0; i < argc; i++)
        vec.push_back(argv[i]);
    return vec;
}