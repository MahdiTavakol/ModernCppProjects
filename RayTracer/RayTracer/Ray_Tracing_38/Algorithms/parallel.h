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
    parallel();
    parallel(std::array<int, 2> rank_config_);
    virtual ~parallel();
    int return_rank() const;
    int return_size() const;
    std::array<int, 2> return_rank_config() const;
    std::array<int, 2> return_size_config() const;
    void barrier() const;
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
