#include "test_shared.h"
#include "../Output/output_parallel.h"
#include "../Data/color_array.h"
#include "../Algorithms/communicator.h"
#include "../Algorithms/mpiComm.h"
#include "../Tests/test_helpers.h"
#include <sstream>

void myRange(
	int width_, int height_,
	std::array<int, 2>& myWidth_, std::array<int, 2>& myHeight_,
	communicator* para_)
{
	auto size = para_->return_size_config();
	auto rank = para_->return_rank_config();

	int sizePerWidth = (width_ + size[0] - 1) / size[0];
	int sizePerHeight = (height_ + size[1] - 1) / size[1];

	int x0 = sizePerWidth * rank[0];
	int x1 = x0 + sizePerWidth;
	int y0 = sizePerHeight * rank[1];
	int y1 = y0 + sizePerHeight;

	x0 = x0 < width_ ? x0 : width_ - 1;
	x1 = x1 <= width_ ? x1 : width_;
	y0 = y0 < height_ ? y0 : height_ - 1;
	y1 = y1 <= height_ ? y1 : height_;

	myWidth_[0] = x0;
	myWidth_[1] = x1;
	myHeight_[0] = y0;
	myHeight_[1] = y1;
}

void distributeColorArray(
	std::unique_ptr<color_array>& c_array_,
	communicator* para_)
{
	int width, height;
	c_array_->return_size(width, height);
	std::array<int, 2> myWidth, myHeight;
	myRange(width, height, myWidth, myHeight, para_);

	int x0 = myWidth[0];
	int x1 = myWidth[1];
	int y0 = myHeight[0];
	int y1 = myHeight[1];

	int newWidth = x1 - x0;
	int newHeight = y1 - y0;

	color_array new_c_array{newWidth,newHeight };

	for (int i = x0; i < x1; i++)
		for (int j = y0; j < y1; j++)
		{
			new_c_array(i-x0,j-y0) = (*c_array_)(i,j);
		}


	*c_array_ = new_c_array;
}

TEST_CASE("Writting a test file in P6 format in parallel","[mpi]")
{
	// the number of ranks for this test
	std::array<int, 2> numRanks = { 2,3 };
	// the parallel object
	// since we want the output_parallel
	// to be run in parallel here we cannot use
	// a fake version of the parallel
	std::unique_ptr<communicator> para = std::make_unique<mpiComm>(MPI_COMM_WORLD);

	// splitting the communicator and skipping the extra ranks
	para = skipExtraRanks(para, numRanks);
	int rank = para->return_rank();


	// filename
	std::string fileName = "empty";


	// color data
	int width, height;
	std::unique_ptr<color_array> c_array;
	color_data** c_data = nullptr;


	SECTION("Test-1")
	{
		fileName = "Output-P6-Test-1-Parallel.ppm";
		width = 192;
		height = 108;

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
		fileName = "Output-P6-Test-2-Parallel.ppm";
		width = 192;
		height = 108;

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
		fileName = "Output-P6-Test-3-Parallel.ppm";
		width = 192;
		height = 108;

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


	c_array = std::make_unique<color_array>(width, height, c_data);
	distributeColorArray(c_array,para.get());
	auto img = std::make_unique<image>(width, height,std::move(c_array), para.get());
	outputMode mode = outputMode::P6;
	output_parallel writer(fileName, std::move(img), para, mode);
	writer.write_file();


	// deallocating the color_array
	deallocate(c_data);


	SUCCEED("Suceeded");
}

TEST_CASE("Testing the output_parallel class","[.][Ignore this for now!]")
{
	// the number of ranks for this test
	std::array<int, 2> numRanks = { 2,2 };
	// I know it is generic 
	// ostringstream and there is no need
	// to feed this into the output classf
	// however I want the c'tor of the 
	// output class to have a std::stringstream
	// on purpose so the user knows 
	// the intention of writing into stringstream
	// rather than a file0

	// streams
	auto dummyOss = std::make_unique<std::stringstream>();
	std::stringstream* oss;
	std::unique_ptr<std::iostream> returnedStream;


	// color data
	int width, height;
	std::unique_ptr<color_array> c_array;
	color_data** c_data = nullptr;

	// the parallel object
	// since we want the output_parallel
	// to be run in parallel here we cannot use
	// a fake version of the parallel
	std::unique_ptr<communicator> para = std::make_unique<mpiComm>(MPI_COMM_WORLD);

	// splitting the communicator and skipping the extra ranks
	para = skipExtraRanks(para, numRanks);



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


	// expectedData
	std::string expectedData(3*width*height,'\0');
	int expectedWidth = width;
	int expectedHeight = height;
	int expectedNColors = 255;

	std::array<int, 2> myWidth, myHeight;
	myRange(width, height, myWidth, myHeight, para.get());
	for (int j = myHeight[0]; j < myHeight[1]; j++)
		for (int i = myWidth[0]; i < myWidth[1]; i++)
		{
			unsigned __int8 r =
				static_cast<unsigned __int8>
				(256 * std::clamp(c_data[i][j].r, 0.0, 0.999));
			unsigned __int8 g =
				static_cast<unsigned __int8>
				(256 * std::clamp(c_data[i][j].g, 0.0, 0.999));
			unsigned __int8 b =
				static_cast<unsigned __int8>
				(256 * std::clamp(c_data[i][j].b, 0.0, 0.999));


			int indx = j * width + i;
			expectedData[3 * indx] = r;
			expectedData[3 * indx + 1] = g;
			expectedData[3 * indx + 2] = b;
		}


	c_array = std::make_unique<color_array>(width, height, c_data);
	auto img = std::make_unique<image>(width, height, std::move(c_array),para.get());
	output_parallel writer(std::move(dummyOss), std::move(img), para);
	writer.write_file();
	returnedStream = writer.return_stream();

	// deallocating the color_array
	deallocate(c_data);

	// converting the returned ostream to ostringstream
	oss = dynamic_cast<std::stringstream*>(returnedStream.get());
	// checking the returned oss pointer type
	REQUIRE(oss);

	int rank = para->return_rank();
	// checking the written data
	REQUIRE_THAT(oss, P6Matcher(&expectedData, expectedWidth, expectedHeight, expectedNColors,rank));
}