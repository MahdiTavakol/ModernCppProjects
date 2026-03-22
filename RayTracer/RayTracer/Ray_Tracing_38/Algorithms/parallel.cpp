#include "parallel.h"

parallel::parallel(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    // MPI variables
    MPI_world = MPI_COMM_WORLD;
    MPI_Comm_rank(MPI_world, &rank);
    MPI_Comm_size(MPI_world, &size);
    rank_config = { size,1 };
}


parallel::parallel(std::array<int, 2> rank_config_) :
    rank_config{ rank_config_ }
{
    MPI_world = MPI_COMM_WORLD;
    MPI_Comm_rank(MPI_world, &rank);
    MPI_Comm_size(MPI_world, &size);
    int ratio = static_cast<int>(size / (size_config[0] * size_config[1]));
    int ratioSq = static_cast<int>(std::sqrt(ratio));
    size_config[0] *= ratioSq;
    size_config[1] = static_cast<int>(size / size_config[0]);

    int rank_x = rank % size_config[0];
    int rank_y = rank / size_config[1];
    rank_config = { rank_x, rank_y };
}

parallel::~parallel()
{
    MPI_Finalize();
}


int parallel::return_rank() const {
    return rank;
}

int parallel::return_size() const {
    return size;
}


std::array<int, 2> parallel::return_rank_config() const
{
    return rank_config;
}

std::array<int, 2> parallel::return_size_config() const
{
    return size_config;
}

void parallel::barrier() const {
    MPI_Barrier(MPI_world);
}
