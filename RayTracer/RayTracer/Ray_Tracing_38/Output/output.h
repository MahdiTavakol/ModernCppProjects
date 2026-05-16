#pragma once

#include <fstream>
#include <functional>

#include "../Shared/rtweekend.h"

#include "../Data/color_array.h"
#include "../Algorithms/image.h"
#include "../Input/settings.h"
#include "../Input/output_settings.h"



class output {
public:
	output(settings* out_settings,
		communicator* para_);
	output(std::string _file_name,
		   communicator* para_,
		   outputMode mode_);
	output(std::unique_ptr<std::iostream> _stream,
		   communicator* para_,
		   outputMode mode_);
	virtual ~output();

	virtual void setup(image* img_);

	void write_file(image* img_) { write_file(img_, binary_pos); }
	virtual void write_file(image* img_, std::streampos pos_);
	void reset_filename(std::string file_name_) { file_name = file_name_; }
	void open_new_file(std::string _file_name);
	void open_file() { open_new_file(file_name); }
	std::unique_ptr<std::iostream> return_stream();
	std::streampos return_binary_begin();
	std::string return_file_name();

protected:
	
	static void remove_file(const std::string& fileName_);
	std::streampos write_header(const int& width_, const int& height_);
	outputMode outMode = outputMode::P3;
	std::string file_name;
	// I want it to be iostream so that output_parallel can read it 
	// to get the begining of the binary section
	std::unique_ptr<std::iostream> stream;
	std::fstream* file = nullptr;


	// running strategy
	communicator* para;



	// the begining of the binary section
	std::streampos binary_pos;

	// size
	std::array<int, 2> size;


	// clamping mode
	ClampMode clmp_mode;
	double exp_factor;

	// clamping functions
	std::function<void(color_data&, double&)> clmp_function;

	// clamping functions
	inline static void simple_clamp(color_data& c_data_, [[maybe_unused]] double& exp_factor_)
	{
		c_data_.r = std::clamp(c_data_.r, 0.0, 0.999);
		c_data_.g = std::clamp(c_data_.g, 0.0, 0.999);
		c_data_.b = std::clamp(c_data_.b, 0.0, 0.999);
	}

	inline static void reinhard(color_data& c_data_, [[maybe_unused]] double& exp_factor_)
	{
		c_data_.r = c_data_.r / (1.0 + c_data_.r);
		c_data_.g = c_data_.g / (1.0 + c_data_.g);
		c_data_.b = c_data_.b / (1.0 + c_data_.b);
	}

	inline static void exponential_tm(color_data& c_data_, double exp_factor_)
	{
		c_data_.r = 1.0 - std::exp(-exp_factor_ * c_data_.r);
		c_data_.g = 1.0 - std::exp(-exp_factor_ * c_data_.g);
		c_data_.b = 1.0 - std::exp(-exp_factor_ * c_data_.b);
	}
};

