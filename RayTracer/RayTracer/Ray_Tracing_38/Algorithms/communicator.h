#pragma once

#include "../Shared/rtweekend.h"


#include <iostream>
#include <mpi.h>
#include <array>
#include "camera.h"
#include "hittable_list.h"
#include "../Data/color_array.h"
#include "image.h"


class communicator
{
public:
    communicator() = default;
	communicator(std::array<int, 2> size_config_);
    communicator(std::array<int, 2> size_config_, std::array<int, 2> rank_config_);
    virtual ~communicator() = default;
    virtual int return_rank() const;
    virtual int return_size() const;
    virtual std::array<int, 2> return_rank_config() const;
    virtual std::array<int, 2> return_size_config() const;
    virtual std::unique_ptr<communicator> split(const std::array<int,2>& maxRanks_) const = 0;
    virtual void gather(int** one_, int** one_all, const int& width_per_rank_, const int& height_per_rank_) const
    {}
    virtual void gather(color_data* one_, color_data* one_all, const int& num_data) const = 0;
    virtual void gather(color_data** one_,
                        color_data** one_all_,
                        const int& width_per_rank_,
                        const int& height_per_rank_) const = 0;
    virtual void gather(
        std::unique_ptr<color_array>& one_,
        std::unique_ptr<color_array>& one_all_,
        std::array<int, 2>& size_per_rank_,
        std::array<int, 2>& size_) const {}
    
    virtual void bcast(void *buff_, int nBytes_, int root_ = 0) const {}
    virtual void barrier() const = 0;

protected:
    int size, rank;
    std::array<int, 2> rank_config;
    std::array<int, 2> size_config;
};

