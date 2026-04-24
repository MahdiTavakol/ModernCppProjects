#include "output_settings.h"
#include <sstream>

output_settings::output_settings(int mode_):
	settings{mode_}
{
	this->type = outputType::SERIAL;
	this->outMode = outputMode::P3;

	set_input_map();
}

void output_settings::set_input_map()
{
	singleInputMap = {
		{"-output_type",&output_type_str},
		{"-output_mode",&output_mode_str},

		{"--o",&output_type_str},
		{"--om",&output_mode_str},
	};
}

void output_settings::parse_commands()
{
	settings::parse_commands();
	this->extra_parse();
}

void output_settings::extra_parse()
{
	if (output_type_str == "SERIAL")
	{
		type = outputType::SERIAL;
	}
	else if (output_type_str == "PARALLEL")
	{
		type = outputType::PARALLEL;
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
}


outputType& output_settings::return_type()
{
	return type;
}
outputMode& output_settings::return_outputMode()
{
	return outMode;
}

std::string output_settings::return_file_name()
{
	return file_name;
}

void output_settings::set_file_name(std::string file_name_)
{
	file_name = file_name_;
}

void output_settings::check_validity() const
{
	settings::check_validity();

	if (type == outputType::PARALLEL)
	{
		if (outMode != outputMode::P3)
			throw std::invalid_argument("Invalid output mode for PARALLEL output type! Only P3 is allowed.");
	}
}