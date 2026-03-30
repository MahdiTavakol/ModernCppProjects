#define CATCH_CONFIG_RUNNER
#include "catch_amalgamated.hpp"


#include "test_shared.h"
#include <sstream>

#include "../Algorithms/mpiParallel.h"

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);


	int result = Catch::Session().run(argc, argv);


	MPI_Finalize();
}

class intMatMatcher : public Catch::Matchers::MatcherGenericBase
{
public:
	intMatMatcher(int** expected_, int size_x_, int size_y_) :
		expected{ expected_ }, size_x{ size_x_ }, size_y{ size_y_ } {
	}
	bool match(int** value_) const
	{
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				if (value_[i][j] != expected[i][j])
					return false;
		return true;
	}
	std::string describe() const override
	{
		std::string output = "A custom 2D array matcher";
		return output;
	}
private:
	int size_x, size_y;
	int** expected;
};

TEST_CASE("Testing the gather function for 2Dimensional rank arrangement")
{
	int width_per_rank = 4;
	int height_per_rank = 3;
	// 3 ranks in the width and 2 ranks in the height
	std::array<int, 2> size_config{ 3,2 };
	int width = width_per_rank * size_config[0];
	int height = height_per_rank * size_config[1];
	std::unique_ptr<parallel> para = std::make_unique<mpiParallel>(size_config);

	int** c_data;
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

	allocate_c_data(width_per_rank, height_per_rank, c_data);
	int rank = para->return_rank();

	int offset = rank * width_per_rank * height_per_rank;
	for (int i = 0; i < width_per_rank; i++)
		for (int j = 0; j < height_per_rank; j++)
			c_data[i][j] = offset + i * height_per_rank + j;


	int** c_data_all_expected;
	allocate_c_data(width, height, c_data_all_expected);

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			int rank_i = i / width_per_rank;
			int rank_j = j / height_per_rank;
			int offset = (rank_i + rank_j * rank_config[0]) * width_per_rank * height_per_rank;
			int local_i = i % width_per_rank;
			int local_j = j % height_per_rank;
			c_data_all_expected[i][j] = offset + local_i * height_per_rank + local_j;
		}



	para->gather(c_data, c_data_all_expected, width_per_rank, height_per_rank);

	// comparing the results
	REQUIRE_THAT(c_data_all_expected, intMatMatcher(c_data_all_expected, width, height));


	deallocate_c_data(c_data);
	deallocate_c_data(c_data_all_expected);

}







