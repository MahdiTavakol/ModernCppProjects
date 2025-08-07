#pragma once
#include <string>
#include <iostream>
#include "mpi.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ostream;
using std::vector;


class Logger {
public:
    Logger() : Logger{ std::vector<std::reference_wrapper<std::ostream>>{std::ref(std::cout)} } {}

    explicit Logger(const std::vector<std::reference_wrapper<std::ostream>>& strms)
        : strm_vec(strms) {
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    }

    Logger(const Logger&) = delete;
    Logger(Logger&&) = default;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = default;
    ~Logger() = default;

    void print(const std::string& input_) const {
        if (rank == 0)
        for (auto& strm : strm_vec) {
            strm.get() << input_ << std::endl;
        }
    }


    template<typename T>
    Logger& operator<<(const T val)
    {
        if (rank == 0)
        {
            for (auto& strm : strm_vec)
            {

                strm.get() << val;
            }
        }
        return *this;
    }

    Logger& operator<<(std::ostream& (*f)(std::ostream&))
    {
        if (rank == 0)
        {
            for (auto& strm : strm_vec)
            {
                strm.get() << f;
            }
        }
        return *this;
    }

private:
    std::vector<std::reference_wrapper<std::ostream>> strm_vec;
    int rank;
};