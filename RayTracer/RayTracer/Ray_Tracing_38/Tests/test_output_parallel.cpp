#include "test_shared.h"
#include "../Output/output_parallel.h"
#include "../Data/color_array.h"
#include <sstream>

void distributeColorArray(
	const int& width_, const int& height_,
	std::unique_ptr<color_array>& c_array_,
	parallel* para_)
{
	auto size = para_->return_size_config();
	auto rank = para_->return_rank_config();

	int sizePerWidth = width_ / size[0];
	int sizePerHeight = height_ / size[1];

	int x0 = (sizePerWidth * rank[0] < width_ ? sizePerWidth * rank[0] : width_ - 1);
	int x1 = (x0 + sizePerWidth < width_ ? x0+sizePerWidth : width_ - 1);
	int y0 = (sizePerHeight * rank[1] < height_ ? sizePerHeight * rank[1] : height_ - 1);
	int y1 = (y0 + sizePerHeight < height_ ? y0 + sizePerHeight : height_ - 1);

	int newWidth = (x1 - x0);
	int newHeight = (y1 - y0);

	color_array new_c_array{newWidth,newHeight };

	for (int i = x0; i < x1; i++)
		for (int j = y0; j < y1; j++)
		{
			new_c_array(i,j) = (*c_array_)(x0 + i,y0 + j);
		}


	*c_array_ = new_c_array;
}

// allocater and deallocator lambda functions
void allocate(color_data**& data_, int width_, int height_)
{
	color_data* temp;
	temp = new color_data[width_ * height_];
	data_ = new color_data * [width_];
	for (int i = 0; i < width_; i++)
		data_[i] = &temp[i * height_];
};
void deallocate(color_data**& data_)
{
	delete[] data_[0];
	delete[] data_;
};

TEST_CASE("Testing the output_parallel class","[.][ignore for now]")
{
	// I know it is generic 
	// ostringstream and there is no need
	// to feed this into the output class
	// however I want the c'tor of the 
	// output class to have a std::ostringstream
	// on purpose so the user knows 
	// the intention of writing into stringstream
	// rather than a file0

	// streams
	auto dummyOss = std::make_unique<std::ostringstream>();
	std::ostringstream* oss;
	std::unique_ptr<std::ostream> returnedStream;


	// color data
	int width, height;
	std::unique_ptr<color_array> c_array;
	color_data** c_data = nullptr;


	SECTION("Test-1")
	{
		width = 100;
		height = 20;

		allocate(c_data, width, height);

		for (int j = 0; j < height; j++)
			for (int i = 0; i < width; i++)
			{
				double r = static_cast<double>(i) / static_cast<double>(width);
				double g = static_cast<double>(j) / static_cast<double>(height);
				double b = 0.0;
				color_data myColor = color_data{ r,g,b };
				c_data[i][j] = myColor;
			}
	}

	SECTION("Test-2")
	{
		width = 1000;
		height = 800;

		allocate(c_data, width, height);

		for (int j = 0; j < height; j++)
			for (int i = 0; i < width; i++)
			{
				double r = static_cast<double>(i) / static_cast<double>(width);
				double g = static_cast<double>(j) / static_cast<double>(height);
				double b = static_cast<double>(j) / static_cast<double>(width);
				color_data myColor = color_data{ r,g,b };
				c_data[i][j] = myColor;
			}
	}

	SECTION("Test-3")
	{
		width = 300;
		height = 1000;

		allocate(c_data, width, height);

		for (int j = 0; j < height; j++)
			for (int i = 0; i < width; i++)
			{
				double r = 0.0;
				double g = static_cast<double>(j) / static_cast<double>(height);
				double b = static_cast<double>(i) / static_cast<double>(width);
				color_data myColor = color_data{ r,g,b };
				c_data[i][j] = myColor;
			}
	}

	// distributing the data among the ranks
	//distributeColorArray(width, height, c_array, );

	// expectedData
	std::vector<std::string> expectedData;
	expectedData.reserve(3 + width * height);
	expectedData.push_back("P3");
	expectedData.push_back(std::to_string(width) + " " + std::to_string(height));
	expectedData.push_back("255");
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			int r = static_cast<int>(256 * std::clamp(c_data[i][j].r, 0.0, 0.999));
			int g = static_cast<int>(256 * std::clamp(c_data[i][j].g, 0.0, 0.999));
			int b = static_cast<int>(256 * std::clamp(c_data[i][j].b, 0.0, 0.999));
			expectedData.push_back(std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b));
		}


	c_array = std::make_unique<color_array>(width, height, c_data);
	output_parallel writer(std::move(dummyOss), c_array.get(), width, height);
	writer.write_file();
	returnedStream = writer.return_stream();

	// converting the returned ostream to ostringstream
	oss = dynamic_cast<std::ostringstream*>(returnedStream.get());
	// checking the returned oss pointer type
	REQUIRE(oss);

	// checking the written data
	REQUIRE_THAT(oss, OStringStreamMatcher(&expectedData));
}