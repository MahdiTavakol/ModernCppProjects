#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "../definitions.h"

#include "test.h"

#include <vector>
#include <string>

using std::vector, std::string;

vector<int> threads = { 1,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40 };

vector<thread_config> thread_cfg_vec = {
    thread_config{1,40},thread_config{2,20},thread_config{4,10},thread_config{5,8}, thread_config{8,5}, thread_config{10,4},
    thread_config{20,2}, thread_config{40,1},
    thread_config{1,36}, thread_config{2,18}, thread_config{4,9}, thread_config{9,4}, thread_config{18,2},thread_config{36,1},
    thread_config{1,32}, thread_config{2,16}, thread_config{4,8}, thread_config{8,4}, thread_config{16,2},thread_config{32,1},
    thread_config{1,20}, thread_config{2,10}, thread_config{4,5}, thread_config{5,4}, thread_config{10,2},thread_config{20,1},
    thread_config{1,16}, thread_config{2,8}, thread_config{4,4}, thread_config{8,2}, thread_config{16,1},
    thread_config{1,8}, thread_config{2,4}, thread_config{4,2}, thread_config{8,1}
};

TEST_CASE("Testing serial running") {
    int expected = 0;

    vector<string> args;
    int area;
    string dummy = "test";
    args.push_back(dummy);
    args.push_back("single");
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

TEST_CASE("Testing various thread numbers") {

    int expected = 0;

    for (const auto& thread : threads) {
        vector<string> args;
        int area;
        string dummy = "test";
        args.push_back(dummy);
        args.push_back("single");
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

TEST_CASE("Testing various thread configurations") {
    int expected = 0;

    for (const auto& thread_cfg : thread_cfg_vec) {
        vector<string> args;
        int area;
        string dummy = "test";
        args.push_back(dummy);
        args.push_back("single");
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
}

