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
    std::array<int, 2> size_config_) :
    parallel{size_config_ }
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

void mpiParallel::gather(int* one_, int* one_all_, const int& width_per_rank_, const int& height_per_rank_) const
{
	int num_data = width_per_rank_ * height_per_rank_;
	int width = width_per_rank_ * size_config[0];
	int height = height_per_rank_ * size_config[1];
    MPI_Allgather(one_, num_data, MPI_INT, one_all_, num_data, MPI_INT, MPI_world);

    int** tempArray;
    int* temp;
	temp = new int[width*height];
	tempArray = new int* [width];
	for (int i = 0; i < width_per_rank_; i++)
        tempArray[i] = &temp[i * height_per_rank_];

	for (int k = 0; k < size; k++)
    {
        int rank_x = k % size_config[0];
        int rank_y = k / size_config[0];
		int rank = rank_y * size_config[0] + rank_x;
        for (int i = 0; i < width_per_rank_; i++)
            for (int j = 0; j < height_per_rank_; j++)
            {
                int offset_i = rank_x * width_per_rank_;
				int offset_j = rank_y * height_per_rank_;
                int global_i = offset_i + i;
				int global_j = offset_j + j;
                // not sure yet
                // one_all shoud get from rank
				tempArray[global_i][global_j] = one_all_[rank*width_per_rank_*height_per_rank_+i*height_per_rank_+j];
            }
    }

}

void mpiParallel::gather(color_data* one_, color_data* one_all_, const int& num_data_) const
{
    MPI_Allgather(one_, num_data_, MPI_DOUBLE, one_all_, num_data_, MPI_DOUBLE, MPI_world);
}

void mpiParallel::barrier() const {
    MPI_Barrier(MPI_world);
}


