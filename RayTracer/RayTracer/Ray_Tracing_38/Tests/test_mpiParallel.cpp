#include "test_shared.h"
#include "../Algorithms/mpiParallel.h"

TEST_CASE("Testing the gather function for 2Dimensional rank arrangement with int** type")
{
	int width_per_rank = 4;
	int height_per_rank = 3;
	// 3 ranks in the width and 2 ranks in the height
	std::array<int, 2> size_config{ 3,2 };
	int width = width_per_rank * size_config[0];
	int height = height_per_rank * size_config[1];
	std::unique_ptr<parallel> para = std::make_unique<mpiParallel>(MPI_COMM_WORLD, size_config);

	int size = para->return_size();
	if (size != 6)
	{
		std::cout << "This test is designed for 6 ranks. Current size: " << size << std::endl;
		std::cout << "Skipping the test." << std::endl;
		return;
	}


	std::array<int, 2> rank_config = para->return_rank_config();

	auto allocate_c_data = [](const int width, const int height, int**& c_data) {
		int* temp = new int[width * height];
		c_data = new int* [width];
		for (int i = 0; i < width; i++)
			c_data[i] = &temp[i * height];
		};

	auto deallocate_c_data = [](int**& c_data) {
		delete[] c_data[0];
		delete[] c_data;
		};


	int** c_data;
	int** c_data_all;
	int** c_data_all_expected;
	allocate_c_data(width_per_rank, height_per_rank, c_data);
	allocate_c_data(width, height, c_data_all);
	allocate_c_data(width, height, c_data_all_expected);




	int rank = para->return_rank();
	int offset = rank * width_per_rank * height_per_rank;
	for (int i = 0; i < width_per_rank; i++)
		for (int j = 0; j < height_per_rank; j++)
			c_data[i][j] = offset + i * height_per_rank + j;


	

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			int rank_i = i / width_per_rank;
			int rank_j = j / height_per_rank;
			int offset = (rank_i + rank_j * size_config[0]) * width_per_rank * height_per_rank;
			int local_i = i % width_per_rank;
			int local_j = j % height_per_rank;
			c_data_all_expected[i][j] = offset + local_i * height_per_rank + local_j;
		}



	para->gather(c_data, c_data_all, width_per_rank, height_per_rank);


	// comparing the results
	REQUIRE_THAT(c_data_all, intMatMatcher(c_data_all_expected, width, height));


	deallocate_c_data(c_data);
	deallocate_c_data(c_data_all);
	deallocate_c_data(c_data_all_expected);
}

TEST_CASE("Testing the gather function for 2Dimensional rank arrangement")
{
	int width_per_rank = 4;
	int height_per_rank = 3;
	// 3 ranks in the width and 2 ranks in the height
	std::array<int, 2> size_config{ 3,2 };
	int width = width_per_rank * size_config[0];
	int height = height_per_rank * size_config[1];
	std::unique_ptr<parallel> para = std::make_unique<mpiParallel>(MPI_COMM_WORLD, size_config);

	int size = para->return_size();
	if (size != 6)
	{
		std::cout << "This test is designed for 6 ranks. Current size: " << size << std::endl;
		std::cout << "Skipping the test." << std::endl;
		return;
	}


	std::array<int, 2> rank_config = para->return_rank_config();



	auto allocate_c_data = [](color_data**& c_data_, const int width_, const int height_) {
		color_data* temp = new color_data[width_ * height_];
		c_data_ = new color_data * [width_];
		for (int i = 0; i < width_; i++)
			c_data_[i] = &temp[i * height_];
	};

	auto deallocate_c_data = [](color_data**& c_data_) {
		delete[] c_data_[0];
		delete[] c_data_;
	};


	color_data** c_data;
	color_data** c_data_all;
	color_data** c_data_all_expected;
	allocate_c_data(c_data, width_per_rank, height_per_rank);
	allocate_c_data(c_data_all, width, height);
	allocate_c_data(c_data_all_expected, width, height);



	int rank = para->return_rank();
	color_data value = color_data{ 0.0,0.0,0.0 };

	auto set_value = [](int rank_) -> color_data
	{
		color_data value = color_data{ 0.0,0.0,0.0 };
		if (rank_ == 0)
			value.r = 1.0;
		else if (rank_ == 1)
			value.g = 1.0;
		else if (rank_ == 2)
			value.b = 1.0;
		else if (rank_ == 3)
			value.r = value.g = 1.0;
		else if (rank_ == 4)
			value.r = value.b = 1.0;
		else if (rank_ == 5)
			value.g = value.b = 1.0;
		return value;
	};

	for (int i = 0; i < width_per_rank; i++)
		for (int j = 0; j < height_per_rank; j++)
			c_data[i][j] = set_value(rank);



	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			int rank_x_ = i / width_per_rank;
			int rank_y_ = j / height_per_rank;
			int rank_ = rank_y_ * size_config[0] + rank_x_;
			c_data_all_expected[i][j] = set_value(rank_);
		}



	para->gather(c_data, c_data_all, width_per_rank, height_per_rank);

	std::unique_ptr<color_array> c_array_all_ptr = 
		std::make_unique<color_array>(width, height, c_data_all);
	std::unique_ptr<color_array> c_array_all_expected_ptr = 
		std::make_unique<color_array>(width, height, c_data_all_expected);


	// comparing the results
	REQUIRE_THAT(c_array_all_ptr.get(), colorArrayMatcher(c_array_all_expected_ptr.get(), 1e-6));


	deallocate_c_data(c_data);
	deallocate_c_data(c_data_all_expected);
}