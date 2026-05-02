#include "../Input/input.h"
#include "test_shared.h"
#include "catch_amalgamated.hpp"


class create_input
{
public:
	create_input(std::vector<std::string> argv_vec_,
		std::vector<std::reference_wrapper<std::ostream>>& outStream_,
		std::unique_ptr<communicator>& para_)
	{
		fill_arg_array(argv_vec_);
		
		std::map<std::string, int> app_set_map =
		{
			{"scene",0},
			{"camera",1},
			{"image",2},
			{"output",3},
			{"renderer",4}
		};

		input_ptr = 
			std::make_unique<input>(
				argc, argv, 
				0,
				app_set_map, para_.get(),
				outStream_);

	}

	create_input(
		std::vector<std::string> argv_vec_,
		std::map<std::string, int> app_set_map_,
		std::unique_ptr<app_settings>&& app_set_,
		std::vector<std::reference_wrapper<std::ostream>>& outStream_,
		std::unique_ptr<communicator>& para_)
	{
		fill_arg_array(argv_vec_);

		input_ptr =
			std::make_unique<input>
			(
				argc,
				argv,
				app_set_map_,
				std::move(app_set_),
				para_.get(),
				outStream_
			);
	}

	~create_input()
	{
		for (int iarg = 0; iarg < argc; iarg++)
		{
			delete[] argv[iarg];
		}
		delete[] argv;
	}

	std::unique_ptr<input> return_input_ptr()
	{
		return std::move(input_ptr);
	}

private:
	char** argv;
	int argc;
	std::unique_ptr<input> input_ptr;


	void fill_arg_array(std::vector<std::string>& argv_vec_)
	{
		// the linear argv vector
		std::vector<std::string> linear_argv_vec;
		
		for (auto& str : argv_vec_)
		{
			std::stringstream ss(str);
			std::string text;
			while (ss >> text)
			{
				linear_argv_vec.push_back(text);
			}
		}

		argc = 1 + static_cast<int>(linear_argv_vec.size());

		argv = new char* [argc];

		argv[0] = new char[5];

		for (int iarg = 1; iarg < argc; iarg++)
		{
			int size = static_cast<int>(linear_argv_vec[iarg - 1].size());
			argv[iarg] = new char[size + 1];
			std::memcpy(argv[iarg], linear_argv_vec[iarg-1].c_str(), size + 1);
		}

		/*
		// the stringstream
		std::stringstream ss;
		argc = static_cast<int>(argv_vec_.size());
		argv = new char* [argc];
		for (int iarg = 0; iarg < argc; iarg++)
		{
			int size = static_cast<int>(argv_vec_[iarg].size());
			argv[iarg] = new char[size + 1];
			std::memcpy(argv[iarg], argv_vec_[iarg].c_str(), size + 1);
		}*/
	}
};

class create_input_file {
	public:
		create_input_file(
			std::vector<std::string>& inputLines
		)
		{
			auto& issObj = *iss;
			for (std::string line : inputLines)
			{
				issObj << line;
			}
		}
		std::unique_ptr<std::stringstream> return_input_file()
		{
			return std::move(iss);
		}


private:
	std::unique_ptr<std::stringstream> iss;
};

TEST_CASE("Testing reading the argv with the input class")
{
	std::vector<std::string> argv_vec;
	std::vector<std::string> expectedLog;

	SECTION("Test-1")
	{
		argv_vec.push_back("renderer");
		argv_vec.push_back("-image_width");
		argv_vec.push_back("640");
		argv_vec.push_back("-samples_per_pixel");
		argv_vec.push_back("240");
		argv_vec.push_back("-max_depth");
		argv_vec.push_back("96");
		argv_vec.push_back("-vfov");
		argv_vec.push_back("45");
		argv_vec.push_back("-aspect_ratio");
		argv_vec.push_back("16");
		argv_vec.push_back("9");
		argv_vec.push_back("-fps");
		argv_vec.push_back("120");
		argv_vec.push_back("-num_seconds");
		argv_vec.push_back("85");


		expectedLog.push_back("The input: ");
		expectedLog.push_back("renderer -image_width 640 -samples_per_pixel 240 ");
		expectedLog.push_back("-max_depth 96 -vfov 45 -aspect_ratio ");
		expectedLog.push_back("16 9 -fps 120 -num_seconds ");
		expectedLog.push_back("85 ");
		expectedLog.push_back("");
		expectedLog.push_back("image_width = 640");
		expectedLog.push_back("samples_per_pixel = 240");
		expectedLog.push_back("max_depth = 96");
		expectedLog.push_back("vfov = 45");
		expectedLog.push_back("aspect_ratio = 16/9");
		expectedLog.push_back("fps = 120");
		expectedLog.push_back("num_seconds = 85");
	}

	SECTION("Test-2")
	{
		argv_vec.push_back("renderer");
		argv_vec.push_back("-image_width");
		argv_vec.push_back("1280");
		argv_vec.push_back("-samples_per_pixel");
		argv_vec.push_back("480");
		argv_vec.push_back("-max_depth");
		argv_vec.push_back("35");
		argv_vec.push_back("-vfov");
		argv_vec.push_back("25");
		argv_vec.push_back("-aspect_ratio");
		argv_vec.push_back("4");
		argv_vec.push_back("3");
		argv_vec.push_back("-fps");
		argv_vec.push_back("20");
		argv_vec.push_back("-num_seconds");
		argv_vec.push_back("15");


		expectedLog.push_back("The input: ");
		expectedLog.push_back("renderer -image_width 1280 -samples_per_pixel 480 ");
		expectedLog.push_back("-max_depth 35 -vfov 25 -aspect_ratio ");
		expectedLog.push_back("4 3 -fps 20 -num_seconds ");
		expectedLog.push_back("15 ");
		expectedLog.push_back("");
		expectedLog.push_back("image_width = 1280");
		expectedLog.push_back("samples_per_pixel = 480");
		expectedLog.push_back("max_depth = 35");
		expectedLog.push_back("vfov = 25");
		expectedLog.push_back("aspect_ratio = 4/3");
		expectedLog.push_back("fps = 20");
		expectedLog.push_back("num_seconds = 15");
	}

	SECTION("Test-3")
	{
		argv_vec.push_back("renderer");
		argv_vec.push_back("-image_width");
		argv_vec.push_back("1920");
		argv_vec.push_back("-samples_per_pixel");
		argv_vec.push_back("256");
		argv_vec.push_back("-max_depth");
		argv_vec.push_back("50");
		argv_vec.push_back("-vfov");
		argv_vec.push_back("40");
		argv_vec.push_back("-aspect_ratio");
		argv_vec.push_back("16");
		argv_vec.push_back("9");
		argv_vec.push_back("-fps");
		argv_vec.push_back("30");
		argv_vec.push_back("-num_seconds");
		argv_vec.push_back("10");


		expectedLog.push_back("The input: ");
		expectedLog.push_back("renderer -image_width 1920 -samples_per_pixel 256 ");
		expectedLog.push_back("-max_depth 50 -vfov 40 -aspect_ratio ");
		expectedLog.push_back("16 9 -fps 30 -num_seconds ");
		expectedLog.push_back("10 ");
		expectedLog.push_back("");
		expectedLog.push_back("image_width = 1920");
		expectedLog.push_back("samples_per_pixel = 256");
		expectedLog.push_back("max_depth = 50");
		expectedLog.push_back("vfov = 40");
		expectedLog.push_back("aspect_ratio = 16/9");
		expectedLog.push_back("fps = 30");
		expectedLog.push_back("num_seconds = 10");
	}



	std::stringstream oss;
	std::vector<std::reference_wrapper<std::ostream>> outStream =
	{
		std::ref(oss)
	};
	std::unique_ptr<communicator> para = std::make_unique<fake_parallel>();
	create_input crt_input(argv_vec, outStream, para);

	REQUIRE_THAT(&oss, StringStreamMatcher(&expectedLog));
}

TEST_CASE("Test reading the input from a file")
{
	std::vector<std::string> inputs;
	// number of settings 
	constexpr int num_settings = 5;
	
	std::vector<std::vector<std::string>> expectedSettings;
	expectedSettings.resize(num_settings);
	// scene - camera - image - output - render
	std::map<std::string, int> settingsMap =
	{
		{"scene",0},
		{"camera",1},
		{"image",2},
		{"output",3},
		{"renderer",4}
	};
	// expected mode
	int expectedMode;

	SECTION("Test-1")
	{
		inputs.push_back("mode RANDOM_SPHERES");
		inputs.push_back("renderer -renderer_mode STATIC");
		inputs.push_back("scene -mode  RANDOM_SPHERES");
		inputs.push_back("camera -samples_per_pixel 72 -max_depth 10 -vfov 45");
		inputs.push_back("image -width 720 -aspect_ratio 16.0 9.0");
		inputs.push_back("output -output_type parallel -output_mode P6 -image_file test_1.ppm");

		expectedMode = RANDOM_SPHERES;
		// scene
		expectedSettings[0].push_back("-mode  RANDOM_SPHERES");
		// camera 
		expectedSettings[1].push_back("-samples_per_pixel 72 -max_depth 10 -vfov 45");
		// image
		expectedSettings[2].push_back("-width 720 -aspect_ratio 16.0 9.0");
		// output
		expectedSettings[3].push_back("-output_type parallel -output_mode P6 -image_file test_1.ppm");
		// render
		expectedSettings[4].push_back("-renderer_mode STATIC");
	}

	SECTION("Test-2")
	{
		inputs.push_back("mode EARTH_SPHERE");
		inputs.push_back("renderer -renderer_mode ANIMATION");
		inputs.push_back("scene -mode  EARTH_SPHERE");
		inputs.push_back("camera -samples_per_pixel 150 -max_depth 30 -vfov 55");
		inputs.push_back("image -width 1920 -aspect_ratio 22.0 18.0");
		inputs.push_back("output -output_type serial -output_mode P3 -image_file test_2.ppm");

		expectedMode = EARTH_SPHERE;
		// scene
		expectedSettings[0].push_back("-mode  EARTH_SPHERE");
		// camera 
		expectedSettings[1].push_back("-samples_per_pixel 150 -max_depth 30 -vfov 55");
		// image
		expectedSettings[2].push_back("-width 1920 -aspect_ratio 22.0 18.0");
		// output
		expectedSettings[3].push_back("-output_type serial -output_mode P3 -image_file test_2.ppm");
		// render
		expectedSettings[4].push_back("-renderer_mode ANIMATION");
	}
	SECTION("Test-3")
	{
		inputs.push_back("mode PERLIN_SPHERE");
		inputs.push_back("renderer -renderer_mode STATIC");
		inputs.push_back("scene -mode  PERLIN_SPHERE");
		inputs.push_back("camera -samples_per_pixel 50 -max_depth 5 -vfov 35");
		inputs.push_back("image -width 1024 -aspect_ratio 4.0 3.0");
		inputs.push_back("output -output_type serial -output_mode P6 -image_file test_3.ppm");

		expectedMode = PERLIN_SPHERE;
		// scene
		expectedSettings[0].push_back("-mode  PERLIN_SPHERE");
		// camera
		expectedSettings[1].push_back("-samples_per_pixel 50 -max_depth 5 -vfov 35");
		// image
		expectedSettings[2].push_back("-width 1024 -aspect_ratio 4.0 3.0");
		// output
		expectedSettings[3].push_back("-output_type serial -output_mode P6 -image_file test_3.ppm");
		// render
		expectedSettings[4].push_back("-renderer_mode STATIC");
	}
	SECTION("Test-4")
	{
		inputs.push_back("mode CORNELL_BOX");
		inputs.push_back("renderer -renderer_mode ANIMATION");
		inputs.push_back("scene -mode  CORNELL_BOX");
		inputs.push_back("camera -samples_per_pixel 300 -max_depth 50 -vfov 40");
		inputs.push_back("image -width 1280 -aspect_ratio 16.0 10.0");
		inputs.push_back("output -output_type parallel -output_mode P3 -image_file test_4.ppm");

		expectedMode = CORNELL_BOX;
		// scene
		expectedSettings[0].push_back("-mode  CORNELL_BOX");
		// camera
		expectedSettings[1].push_back("-samples_per_pixel 300 -max_depth 50 -vfov 40");
		// image
		expectedSettings[2].push_back("-width 1280 -aspect_ratio 16.0 10.0");
		// output
		expectedSettings[3].push_back("-output_type parallel -output_mode P3 -image_file test_4.ppm");
		// render
		expectedSettings[4].push_back("-renderer_mode ANIMATION");
	}

	std::vector<std::unique_ptr<settings>> settingsVec;
	settingsVec.resize(num_settings);
	for (int i = 0; i < num_settings; i++)
	{
		settingsVec[i] = std::move(std::make_unique<fake_settings>());
	}

	std::unique_ptr<app_settings> app_set =
		std::make_unique<fake_app_settings>(
			0,
			std::move(settingsVec),
			settingsMap
		);

	std::stringstream oss;
	std::vector<std::reference_wrapper<std::ostream>> outStream =
	{
		std::ref(oss)
	};

	std::unique_ptr<communicator> para = std::make_unique<fake_parallel>();
	create_input crt_input(
		inputs,
		settingsMap,
		std::move(app_set),
		outStream,
		para);

	
	std::unique_ptr<input> in = crt_input.return_input_ptr();
	in->parse_file();
	std::unique_ptr<app_settings> returned_set = in->return_app_settings();
	fake_app_settings* sett = dynamic_cast<fake_app_settings*>(returned_set.get());
	REQUIRE(sett);
	

	// checking the commands sent to each of the settings in the app_settings
	for (auto& [key, indx] : settingsMap)
	{
		auto& settRef = *sett;
		std::vector<std::string> cmdVec;
		sett->return_cmd(key, cmdVec);
		auto& expected = expectedSettings[indx];
		REQUIRE_THAT(cmdVec, StringVecMatcher(expected));
	}

}