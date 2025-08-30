#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include "mpi.h"

#define LOG_LEVEL_ERROR   0
#define LOG_LEVEL_WARN    1
#define LOG_LEVEL_INFO    2
#define LOG_LEVEL_DEBUG   3
#define LOG_LEVEL_TRACE   4
#define LOG_LEVEL_VERBOSE 5
#define LOG_LEVEL_ALL     6

class Logger {
public:
    // Default: log to std::cout at LOG_LEVEL_ALL
    Logger()
        : Logger(std::vector<std::reference_wrapper<std::ostream>>{ std::ref(std::cout) },
            std::vector<int>{ LOG_LEVEL_ALL }) {
    }

    // Constructor with streams only  all use LOG_LEVEL_ALL
    Logger(const std::vector<std::reference_wrapper<std::ostream>>& strms)
        : Logger(strms, std::vector<int>(strms.size(), LOG_LEVEL_ALL)) {
    }

    // Full constructor
    Logger(const std::vector<std::reference_wrapper<std::ostream>>& strms,
        const std::vector<int>& log_levels_)
        : strm_vec(strms), log_levels(log_levels_) {
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    }

    Logger(const Logger&) = delete;
    Logger(Logger&&) = default;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;
    ~Logger() = default;

    void print(const std::string& input_, const int log_level_ = LOG_LEVEL_ERROR) const {
        if (rank == 0) {
            for (size_t i = 0; i < strm_vec.size(); ++i) {
                if (log_level_ >= log_levels[i])
                    strm_vec[i].get() << input_ << std::endl;
            }
        }
    }

    template<typename T>
    Logger& operator<<(const T& val) {
        if (rank == 0) {
            for (auto& strm : strm_vec)
                strm.get() << val;
        }
        return *this;
    }

    Logger& operator<<(std::ostream& (*f)(std::ostream&)) {
        if (rank == 0) {
            for (auto& strm : strm_vec)
                strm.get() << f;
        }
        return *this;
    }

    const int log_level = LOG_LEVEL_INFO;

private:
    std::vector<std::reference_wrapper<std::ostream>> strm_vec;
    std::vector<int> log_levels;
    int rank;
};
