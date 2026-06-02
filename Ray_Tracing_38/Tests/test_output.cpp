#include "test_shared.h"
#include "../Output/output.h"
#include "../Output/output_serial.h"
#include "../Data/color_array.h"
#include "../Tests/test_helpers.h"
#include <sstream>
#include <stdint.h>
#include <cstddef>
#include <bitset>



TEST_CASE("Testing the output class")
{
	// I know it is generic 
	// ostringstream and there is no need
	// to feed this into the output class
	// however I want the c'tor of the 
	// output class to have a std::stringstream
	// on purpose so the user knows 
	// the intention of writing into stringstream
	// rather than a file

	// streams
	auto dummyOss = std::make_unique<std::stringstream>();
	std::stringstream* oss;
	std::unique_ptr<std::ostream> returnedStream;


	// color data
	int width, height;
	std::unique_ptr<color_array> c_array;
	color_data** c_data = nullptr;
	

	SECTION("Test-1")
	{
		width = 16;
		height = 9;

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
		width = 32;
		height = 12;

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
		width = 48;
		height = 12;

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
	std::unique_ptr<communicator> para = std::make_unique<fake_parallel>();
	std::array<int, 2> imgSize{ width,height };
	auto img = std::make_unique<image>(imgSize, std::move(c_array), para.get());
	output_serial writer(std::move(dummyOss), std::move(img),para);
	writer.write_file();
	returnedStream = writer.return_stream();

	// deallocating the color_array
	deallocate(c_data);

	// converting the returned ostream to ostringstream
	oss = dynamic_cast<std::stringstream*>(returnedStream.get());
	// checking the returned oss pointer type
	REQUIRE(oss);

	// checking the written data
	REQUIRE_THAT(oss, StringStreamMatcher(&expectedData));
}

TEST_CASE("Test writing in the P6 format")
{
	// streams
	auto dummyOss = std::make_unique<std::stringstream>();
	std::stringstream* oss;
	std::unique_ptr<std::iostream> returnedStream;


	// color data
	int width, height;
	std::unique_ptr<color_array> c_array;
	color_data** c_data = nullptr;


	SECTION("Test-1")
	{
		width = 16;
		height = 9;

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
		width = 32;
		height = 12;

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
		width = 48;
		height = 12;

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
	std::vector<std::string> expectedData;
	expectedData.reserve(3 + width * height);
	expectedData.push_back("P6");
	expectedData.push_back(std::to_string(width) + " " + std::to_string(height));
	expectedData.push_back("255");
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			unsigned __int8 r =
				static_cast<unsigned __int8>(256 * std::clamp(c_data[i][j].r, 0.0, 0.999));
			unsigned __int8 g =
				static_cast<unsigned __int8>(256 * std::clamp(c_data[i][j].g, 0.0, 0.999));
			unsigned __int8 b =
				static_cast<unsigned __int8>(256 * std::clamp(c_data[i][j].b, 0.0, 0.999));
			expectedData.emplace_back(1, r);
			expectedData.emplace_back(1, g);
			expectedData.emplace_back(1, b);
		}


	c_array = std::make_unique<color_array>(width, height, c_data);
	std::unique_ptr<communicator> para = std::make_unique<fake_parallel>();
	std::array<int, 2> imgSize{ width,height };
	auto img = std::make_unique<image>(imgSize, std::move(c_array),para.get());
	outputMode mode = outputMode::P6;
	output_serial writer(std::move(dummyOss),std::move(img),para,mode);
	returnedStream = writer.return_stream();

	// deallocating the color_array
	deallocate(c_data);

	// converting the returned ostream to ostringstream
	oss = dynamic_cast<std::stringstream*>(returnedStream.get());
	// checking the returned oss pointer type
	REQUIRE(oss);

	// checking the written data
	REQUIRE_THAT(oss, StringStreamMatcher(&expectedData));
}

TEST_CASE("Writting a test file in P3 format")
{
	// filename
	std::string fileName = "empty";


	// color data
	int width, height;
	std::unique_ptr<color_array> c_array;
	color_data** c_data = nullptr;


	SECTION("Test-1")
	{
		fileName = "Output-P3-Test-1.ppm";
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
		fileName = "Output-P3-Test-2.ppm";
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
		fileName = "Output-P3-Test-3.ppm";
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
	std::unique_ptr<communicator> para = std::make_unique<fake_parallel>();
	std::array<int, 2> imgSize{ width,height };
	auto img = std::make_unique<image>(imgSize, std::move(c_array),para.get());
	outputMode mode = outputMode::P3;
	output_serial writer(fileName, std::move(img), para, mode);
	writer.write_file();

	// deallocating the color_array
	deallocate(c_data);


	SUCCEED("Suceeded");
}

TEST_CASE("Writting a test file in P6 format")
{
	// filename
	std::string fileName = "empty";


	// color data
	int width, height;
	std::unique_ptr<color_array> c_array;
	color_data** c_data = nullptr;


	SECTION("Test-1")
	{
		fileName = "Output-P6-Test-1.ppm";
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
		fileName = "Output-P6-Test-2.ppm";
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
		fileName = "Output-P6-Test-3.ppm";
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
	std::unique_ptr<communicator> para = std::make_unique<fake_parallel>();
	std::array<int, 2> imgSize{ width,height };
	auto img = std::make_unique<image>(imgSize, std::move(c_array),para.get());
	outputMode mode = outputMode::P6;
	output_serial writer(fileName, std::move(img), para, mode);
	writer.write_file();


	// deallocating the color_array
	deallocate(c_data);


	SUCCEED("Suceeded");
}