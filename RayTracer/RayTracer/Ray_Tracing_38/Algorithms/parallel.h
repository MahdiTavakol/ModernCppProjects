#ifndef PARALLEL_H
#define PARALLEL_H

#include "../Shared/rtweekend.h"


#include <iostream>
#include <mpi.h>
#include <array>
#include "camera.h"
#include "hittable_list.h"
#include "hittable_list_parallel.h"
#include "../Data/color_array.h"


class parallel
{
public:
    parallel(int argc, char** argv);
    parallel(std::array<int, 2> rank_config_, std::array<int,2> size_config_);
    virtual ~parallel();
    virtual int return_rank() const;
    virtual int return_size() const;
    virtual std::array<int, 2> return_rank_config() const;
    virtual std::array<int, 2> return_size_config() const;
    virtual void barrier() const;
    MPI_Comm* return_comm()
    {
        return &MPI_world;
    }

protected:
    int size, rank;
    std::array<int, 2> rank_config;
    std::array<int, 2> size_config;
    MPI_Comm MPI_world;
};

#endif
