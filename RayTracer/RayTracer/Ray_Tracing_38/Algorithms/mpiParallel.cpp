#include "mpiParallel.h"

mpiParallel::mpiParallel(int argc, char** argv):
    parallel{}
{
    MPI_Init(&argc, &argv);
    // MPI variables
    MPI_world = MPI_COMM_WORLD;
    MPI_Comm_rank(MPI_world, &rank);
    MPI_Comm_size(MPI_world, &size);
    rank_config = { rank, 0 };
    size_config = { size, 1 };
}


mpiParallel::mpiParallel(
    std::array<int, 2> rank_config_,
    std::array<int, 2> size_config_) :
    parallel{ rank_config_ , size_config_ }
{
    MPI_world = MPI_COMM_WORLD;
    MPI_Comm_rank(MPI_world, &rank);
    MPI_Comm_size(MPI_world, &size);
    int ratio = static_cast<int>(size / (size_config[0] * size_config[1]));
    int ratioSq = static_cast<int>(std::sqrt(ratio));
    size_config[0] *= ratioSq;
    size_config[1] = static_cast<int>(size / size_config[0]);

    int rank_x = rank % size_config[0];
    int rank_y = rank / size_config[0];
    rank_config = { rank_x, rank_y };
}

mpiParallel::~mpiParallel()
{
    MPI_Finalize();
}

void mpiParallel::gather(color_data* one_, color_data* one_all_, const int& num_data_) const
{
    MPI_Allgather(one_, num_data_, MPI_DOUBLE, one_all_, num_data_, MPI_DOUBLE, MPI_world);
}

void mpiParallel::barrier() const {
    MPI_Barrier(MPI_world);
}


