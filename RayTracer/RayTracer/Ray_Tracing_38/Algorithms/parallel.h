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
    parallel() = default;
	parallel(std::array<int, 2> size_config_);
    virtual ~parallel() = default;
    virtual int return_rank() const;
    virtual int return_size() const;
    virtual std::array<int, 2> return_rank_config() const;
    virtual std::array<int, 2> return_size_config() const;
    virtual void gather(int** one_, int** one_all, const int& width_per_rank_, const int& height_per_rank_) const
    {}
    virtual void gather(color_data* one_, color_data* one_all, const int& num_data) const = 0;
    virtual void gather(color_data** one_,
                        color_data** one_all_,
                        const int& width_per_rank_,
                        const int& height_per_rank_) const = 0;
    virtual void barrier() const = 0;

protected:
    int size, rank;
    std::array<int, 2> rank_config;
    std::array<int, 2> size_config;
};

#endif
