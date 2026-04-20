#pragma once
#include "parallel.h"

class mpiParallel : public parallel
{
public:
    explicit mpiParallel(MPI_Comm comm_);
    explicit mpiParallel(MPI_Comm comm_,std::array<int, 2> size_config_);
    explicit mpiParallel(MPI_Comm comm_, std::array<int, 2> size_config_, std::array<int, 2> rank_config_);
    virtual ~mpiParallel() override;
    virtual std::unique_ptr<parallel> split(const std::array<int,2>& maxRanks_) const override;
    virtual void gather(int** one_, int** one_all, const int& width_per_rank_, const int& height_per_rank_) const override;
    virtual void gather(color_data* one_, color_data* one_all, const int& num_data) const override;
    virtual void gather(color_data** one_,
                        color_data** one_all_,
                        const int& width_per_rank_,
                        const int& height_per_rank_) const override;
    void gather(
        std::unique_ptr<color_array>& one_,
        std::unique_ptr<color_array>& one_all_,
        std::array<int,2>& size_per_rank_,
        std::array<int,2>& size_) const override;
    void gather(
        std::unique_ptr<image>& one_,
        std::unique_ptr<image>& one_all_
    ) const override;
    virtual void barrier() const override;
    void bcast(void* buff_, int nBytes_, int root_ = 0) const override;
    MPI_Comm* return_comm()
    {
        return &MPI_world;
    }

protected:
    MPI_Comm MPI_world;
};