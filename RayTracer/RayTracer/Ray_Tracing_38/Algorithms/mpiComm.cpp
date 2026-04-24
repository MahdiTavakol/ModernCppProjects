#include "mpiComm.h"

mpiComm::mpiComm(MPI_Comm comm_):
    communicator{},
    MPI_world{comm_}
{
    // MPI variables
    MPI_Comm_rank(MPI_world, &rank);
    MPI_Comm_size(MPI_world, &size);
    rank_config = { rank, 0 };
    size_config = { size, 1 };
}


mpiComm::mpiComm(
    MPI_Comm comm_,
    std::array<int, 2> size_config_) :
    communicator{size_config_ },
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


mpiComm::mpiComm(
    MPI_Comm comm_,
    communicator_settings* stngs_) :
    communicator{ stngs_ },
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

mpiComm::mpiComm(
    MPI_Comm comm_, 
    std::array<int, 2> size_config_,
    std::array<int, 2> rank_config_):
    communicator{size_config_,rank_config_},
    MPI_world{comm_}
{
    MPI_Comm_rank(MPI_world, &rank);
    MPI_Comm_size(MPI_world, &size);
}


mpiComm::~mpiComm()
{}

void mpiComm::gather(int** one_, int** one_all_, const int& width_per_rank_, const int& height_per_rank_) const
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

std::unique_ptr<communicator> mpiComm::split(const std::array<int, 2>& maxRanks_) const
{
    int newColor;
    int newRank = 0;
    std::array<int, 2> newRankConfig;
    std::array<int, 2> newSizeConfig;

    int maxNumRanks = maxRanks_[0] * maxRanks_[1];



    if (rank < maxNumRanks)
    {
        newColor = 0;
        newRank = rank;
        newRankConfig[0] = rank % maxRanks_[0];
        newRankConfig[1] = rank / maxRanks_[0];
        newSizeConfig = maxRanks_;
    }
    else
    {
        newColor = 1;
        newRank = rank - maxNumRanks;
        newRankConfig[0] = newRank;
        newRankConfig[1] = 0;
        newSizeConfig[0] = size - maxNumRanks;
        newSizeConfig[1] = 1;
    }


    MPI_Comm newComm;
    MPI_Comm_split(MPI_world, newColor, newRank, &newComm);

    auto out = std::make_unique<mpiComm>(newComm, newSizeConfig, newRankConfig);

    return out;
}


void mpiComm::gather(color_data** one_,
                         color_data** one_all_,
                         const int& width_per_rank_,
                         const int& height_per_rank_) const
{
	int num_bytes = (width_per_rank_ * height_per_rank_) * sizeof(color_data);
    MPI_Allgather(one_[0], num_bytes, MPI_BYTE, one_all_[0], num_bytes, MPI_BYTE, MPI_world);


    int width = width_per_rank_ * size_config[0];
    int height = height_per_rank_ * size_config[1];


    color_data** tempArray;
    color_data* temp;
    temp = new color_data[width * height];
    tempArray = new color_data* [width];
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

void mpiComm::gather(color_data* one_, color_data* one_all_, const int& num_data_) const
{
    MPI_Allgather(one_, num_data_, MPI_DOUBLE, one_all_, num_data_, MPI_DOUBLE, MPI_world);
}

void mpiComm::gather(
    std::unique_ptr<color_array>& one_,
    std::unique_ptr<color_array>& one_all_,
    std::array<int, 2>& size_per_rank_,
    std::array<int, 2>& size_) const
{
	int width_per_rank = size_per_rank_[0];
	int height_per_rank = size_per_rank_[1];
	int image_width = size_[0];
	int image_height = size_[1];

    int extended_width = width_per_rank * size_config[0];
    int extended_height = height_per_rank * size_config[1];

    color_data** colors = one_->return_array();
    color_data** colors_all = (color_data**)malloc(extended_width * sizeof(color_data*));
    color_data* temp = (color_data*)malloc(extended_width * extended_height * sizeof(color_data));
    for (int i = 0; i < extended_width; i++)
        colors_all[i] = &temp[i * extended_height];


    gather(colors, colors_all, width_per_rank, height_per_rank);


    // a copy of data is involved here I should work on removing this!
    one_all_ =
        std::make_unique<color_array>(image_width, image_height, colors_all);

    free(colors_all[0]);
    free(colors_all);
    colors_all = nullptr;
}


void mpiComm::barrier() const {
    MPI_Barrier(MPI_world);
}

void mpiComm::bcast(void* buff_, int nBytes_, int root_) const
{
    MPI_Bcast(buff_, nBytes_, MPI_BYTE, root_, MPI_world);
}


