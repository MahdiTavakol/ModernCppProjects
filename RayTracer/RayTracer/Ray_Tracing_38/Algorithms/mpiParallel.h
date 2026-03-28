#pragma once
#include "parallel.h"

class mpiParallel : public parallel
{
public:
    mpiParallel(int argc, char** argv);
    mpiParallel(std::array<int, 2> rank_config_, std::array<int, 2> size_config_);
    virtual ~mpiParallel() override;
    virtual void gather(color_data* one_, color_data* one_all, const int& num_data) const override;
    virtual void barrier() const override;
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