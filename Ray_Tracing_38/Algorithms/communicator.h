#pragma once

#include "../Shared/rtweekend.h"


#include <iostream>
#include <array>
#include "../Data/color_array.h"
#include "../Input/communicator_settings.h"


class communicator
{
public:
    communicator() = default;
	communicator(std::array<int, 2> size_config_);
    communicator(settings* comm_settings_);
    communicator(std::array<int, 2> size_config_, std::array<int, 2> rank_config_);
    virtual ~communicator() = default;
    virtual int return_rank() const;
    virtual int return_size() const;
    virtual void reset_size_ratio(std::array<int,2> new_size_ratio_);
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
    int size = 1, rank = 0;
    std::array<int, 2> rank_config = { 0,0 };
    std::array<int, 2> size_config = { 1,1 };
};

