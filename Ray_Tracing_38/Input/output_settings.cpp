#include "output_settings.h"
#include <sstream>

output_settings::output_settings(int mode_):
	settings{mode_}
{
	set_mode(mode);
	set_input_map();
}

void output_settings::set_input_map()
{
	singleInputMap = {
		{"-output_type",&output_type_str},
		{"-output_mode",&output_mode_str},
		{"-image_file", &file_name},
		{"-clamp_mode",&clmp_mode_str},
		{"-exp_factor",&exp_factor},

		{"--o",&output_type_str},
		{"--om",&output_mode_str},
	};

	doubleInputMap = {
		{"-image_size",std::pair{&size[0],&size[1]}}
	};
}

void output_settings::set_mode(int mode_)
{
	mode = mode_;

	switch (mode)
	{
	case SIMPLE_2D_PARALEL_TEST:
		type = outputType::PARALLEL;
		outMode = outputMode::P6;
		break;
	default:
		this->type = outputType::SERIAL;
		this->outMode = outputMode::P3;
	}

}

void output_settings::parse_commands()
{
	settings::parse_commands();
	this->extra_parse();
}

void output_settings::extra_parse()
{
	std::transform(output_type_str.begin(), output_type_str.end(), output_type_str.begin(), ::toupper);
	std::transform(output_mode_str.begin(), output_mode_str.end(), output_mode_str.begin(), ::toupper);
	std::transform(clmp_mode_str.begin(), clmp_mode_str.end(), clmp_mode_str.begin(), ::toupper);


	if (output_type_str == "SERIAL")
	{
		type = outputType::SERIAL;
	}
	else if (output_type_str == "PARALLEL")
	{
		type = outputType::PARALLEL;
	}
	else if (output_type_str == "ASYNC")
	{
		type = outputType::ASYNC;
	}
	else
	{
		std::cerr << "Invalid output mode: " << output_type_str << std::endl;
	}

	if (output_mode_str == "P3")
	{
		outMode = outputMode::P3;
	}
	else if (output_mode_str == "P6")
	{
		outMode = outputMode::P6;
	}
	else
	{
		std::cerr << "Invalid output mode: " << output_mode_str << std::endl;
	}

	if (clmp_mode_str == "SIMPLE")
	{
		clmp_mode = ClampMode::SIMPLE;
	}
	else if (clmp_mode_str == "REINHARD")
	{
		clmp_mode = ClampMode::REINHARD;
	}
	else if (clmp_mode_str == "EXPONENTIAL")
	{
		clmp_mode = ClampMode::EXPONENTIAL;
	}



}


outputType output_settings::return_type()
{
	return type;
}
outputMode output_settings::return_outputMode()
{
	return outMode;
}

std::string output_settings::return_file_name()
{
	return file_name;
}

std::array<int, 2> output_settings::return_size() const
{
	return size;
}

void output_settings::check_validity() const
{
	if (type == outputType::PARALLEL)
	{
		if (outMode != outputMode::P6)
			throw std::invalid_argument("Invalid output mode for PARALLEL output type! Only P6 is allowed.");
	}
}

void output_settings::log_class_name(std::iostream& stream_) const
{
	stream_ << "Output class options:" << std::endl << std::endl;
}