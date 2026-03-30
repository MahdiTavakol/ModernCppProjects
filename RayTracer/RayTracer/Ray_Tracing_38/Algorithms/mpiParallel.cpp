#include "mpiParallel.h"

mpiParallel::mpiParallel(MPI_Comm comm_):
    parallel{},
    MPI_world{comm_}
{
    // MPI variables
    MPI_Comm_rank(MPI_world, &rank);
    MPI_Comm_size(MPI_world, &size);
    rank_config = { rank, 0 };
    size_config = { size, 1 };
}


mpiParallel::mpiParallel(
    MPI_Comm comm_,
    std::array<int, 2> size_config_) :
    parallel{size_config_ },
	MPI_world{ comm_ }
{
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
}

void mpiParallel::gather(int** one_, int** one_all_, const int& width_per_rank_, const int& height_per_rank_) const
{
	int num_data = width_per_rank_ * height_per_rank_;
	int width = width_per_rank_ * size_config[0];
	int height = height_per_rank_ * size_config[1];
    MPI_Allgather(one_[0], num_data, MPI_INT, one_all_[0], num_data, MPI_INT, MPI_world);

    int** tempArray;
    int* temp;
	temp = new int[width*height];
	tempArray = new int* [width];
	for (int i = 0; i < width; i++)
        tempArray[i] = &temp[i * height];

	for (int k = 0; k < size; k++)
    {
        int rank_k = k;
		int rank_x_k = rank_k % size_config[0];
		int rank_y_k = rank_k / size_config[0];
        for (int i = 0; i < width_per_rank_; i++)
            for (int j = 0; j < height_per_rank_; j++)
            {
				int in = rank_k * width_per_rank_ * height_per_rank_ + i * height_per_rank_ + j;
				int out_i = rank_x_k * width_per_rank_ + i;
				int out_j = rank_y_k * height_per_rank_ + j;
				tempArray[out_i][out_j] = one_all_[0][in];
            }
    }

	std::copy_n(tempArray[0], width * height, one_all_[0]);


	delete[] tempArray[0];
	delete[] tempArray;
}

void mpiParallel::gather(color_data* one_, color_data* one_all_, const int& num_data_) const
{
    MPI_Allgather(one_, num_data_, MPI_DOUBLE, one_all_, num_data_, MPI_DOUBLE, MPI_world);
}

void mpiParallel::barrier() const {
    MPI_Barrier(MPI_world);
}


