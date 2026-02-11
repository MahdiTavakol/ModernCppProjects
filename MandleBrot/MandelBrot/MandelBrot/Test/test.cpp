#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "../definitions.h"
#include "../Runner/run_mandelbrot_timing.h"

#include "test.h"

#include <vector>
#include <map>
#include <string>

using std::vector, std::string, std::map;

static map<int, int> center_id_results = { {1,759168}, {2,487535}, {3,487535},
                                           {4,507003}, {5,524284}, {6,763938},
                                           {7,759132}, {8,525352}, {9,507009},
                                           {10,411034}, {11,300908} };

static vector<int> threads = { 1,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40 };

static vector<thread_config> thread_cfg_vec = {
    // 40 threads
    thread_config{1,40}, thread_config{2,20}, thread_config{4,10},
    thread_config{5,8},  thread_config{8,5},  thread_config{10,4},
    thread_config{20,2}, thread_config{40,1},
    // 36 threads
    thread_config{1,36}, thread_config{2,18}, thread_config{4,9}, 
    thread_config{9,4},  thread_config{18,2}, thread_config{36,1}, 
    // 32 threads
    thread_config{1,32}, thread_config{2,16}, thread_config{4,8}, 
    thread_config{8,4},  thread_config{16,2}, thread_config{32,1}, 
    // 20 threads
    thread_config{1,20}, thread_config{2,10}, thread_config{4,5},
    thread_config{5,4},  thread_config{10,2}, thread_config{20,1},
    // 16 threads
    thread_config{1,16}, thread_config{2,8},  thread_config{4,4},
    thread_config{8,2},  thread_config{16,1},
    // 8 threads
    thread_config{1,8},  thread_config{2,4},  thread_config{4,2},
    thread_config{8,1}
};

static std::vector<allocation_mode> alloc_mode_vec = { allocation_mode::C, allocation_mode::CPP, allocation_mode::MODERN};
static std::vector<allocation_major> alloc_major_vec = { allocation_major::X_MAJOR, allocation_major::Y_MAJOR };
static std::vector<Mesh_type> mesh_vec = { Mesh_type::SERIAL, Mesh_type::INNER_LOOP, Mesh_type::OUTER_LOOP };


TEST_CASE("Testing serial running with various centers (the single mode)") {

    for (const auto& [type, expected] : center_id_results) {
        vector<string> args;
        int area;
        string dummy = "test";
        args.push_back(dummy);
        args.push_back("single");
        args.push_back("--type");
        args.push_back(std::to_string(type));
        args.push_back("--info");
        args.push_back("test");
        args.push_back("--resolution");
        args.push_back(std::to_string(1920));
        args.push_back(std::to_string(1080));
        args.push_back("--threads");
        args.push_back(std::to_string(1));
        args.push_back(std::to_string(1));
        area = run_wrapper(args);


        REQUIRE(area == expected);
    }
}

TEST_CASE("Testing serial running (the single mode)") {
    int expected = center_id_results[1];

    vector<string> args;
    int area;
    string dummy = "test";
    args.push_back(dummy);
    args.push_back("single");
    args.push_back("--type");
    args.push_back(std::to_string(1));
    args.push_back("--info");
    args.push_back("test");
    args.push_back("--resolution");
    args.push_back(std::to_string(1920));
    args.push_back(std::to_string(1080));
    args.push_back("--threads");
    args.push_back(std::to_string(1));
    args.push_back(std::to_string(1));
    area = run_wrapper(args);

    REQUIRE(area == expected);
}

TEST_CASE("Testing various thread numbers (The single mode)") {

    int expected = center_id_results[1];

    for (const auto& thread : threads) {
        vector<string> args;
        int area;
        string dummy = "test";
        args.push_back(dummy);
        args.push_back("single");
        args.push_back("--type");
        args.push_back(std::to_string(1));
        args.push_back("--info");
        args.push_back("test");
        args.push_back("--resolution");
        args.push_back(std::to_string(1920));
        args.push_back(std::to_string(1080));
        args.push_back("--threads");
        args.push_back(std::to_string(thread));
        args.push_back(std::to_string(1));
        area = run_wrapper(args);

        REQUIRE(area == expected);
    }
}

TEST_CASE("Testing various thread configurations (The single mode)") {

    int expected = center_id_results[1];

    for (const auto& thread_cfg : thread_cfg_vec) {
        vector<string> args;
        int area;
        string dummy = "test";
        args.push_back(dummy);
        args.push_back("single");
        args.push_back("--type");
        args.push_back(std::to_string(1));
        args.push_back("--info");
        args.push_back("test");
        args.push_back("--resolution");
        args.push_back(std::to_string(1920));
        args.push_back(std::to_string(1080));
        args.push_back("--threads");
        args.push_back(std::to_string(thread_cfg.threads_x));
        args.push_back(std::to_string(thread_cfg.threads_y));
        area = run_wrapper(args);

        REQUIRE(area == expected);

    }

    for (const auto& thread : threads) {

    }
}

TEST_CASE("Testing various thread numbers (The timing mode)") {
    int expected = center_id_results[1];

    vector<allocation_mode> alloc_mode_vec_i;
    vector<allocation_major> alloc_major_vec_i;
    vector<thread_config> thread_cfg_vec_i;
    vector<Mesh_type> mesh_vec_i;
    alloc_mode_vec_i.push_back(alloc_mode_vec[0]);
    alloc_major_vec_i.push_back(alloc_major_vec[0]);
    mesh_vec_i.push_back(mesh_vec[0]);
    
    for (auto& thread : threads)
        thread_cfg_vec_i.push_back(thread_config(thread, 1));


    vector<string> args;
    string dummy = "test";
    args.push_back(dummy);
    args.push_back("timing");
    args.push_back("--type");
    args.push_back(std::to_string(1));
    args.push_back("--info");
    args.push_back("test");
    args.push_back("--resolution");
    args.push_back(std::to_string(1920));
    args.push_back(std::to_string(1080));
    args.push_back("--noWrite");

    std::unique_ptr<Runner_NS::run_mandelbrot_timing> runner =
        std::make_unique<Runner_NS::run_mandelbrot_timing>(args);
    runner->reset_setting_map(alloc_mode_vec_i, alloc_major_vec_i, thread_cfg_vec_i, mesh_vec_i);
    runner->run();
    std::vector<int> outs = runner->return_area_vec();

    for (const auto& out : outs)
        REQUIRE(out == expected);
}


TEST_CASE("Testing various thread configurations (The timing mode)") {
    int expected = center_id_results[1];

    vector<string> args;
    string dummy = "test";
    args.push_back(dummy);
    args.push_back("timing");
    args.push_back("--type");
    args.push_back(std::to_string(1));
    args.push_back("--info");
    args.push_back("test");
    args.push_back("--resolution");
    args.push_back(std::to_string(1920));
    args.push_back(std::to_string(1080));
    args.push_back("--noWrite");

    std::unique_ptr<Runner_NS::run_mandelbrot_timing> runner =
        std::make_unique<Runner_NS::run_mandelbrot_timing>(args);
    runner->reset_setting_map(alloc_mode_vec, alloc_major_vec, thread_cfg_vec, mesh_vec);
    runner->run();
    std::vector<int> outs = runner->return_area_vec();

    for (const auto& out : outs)
        REQUIRE(out == expected);
}

