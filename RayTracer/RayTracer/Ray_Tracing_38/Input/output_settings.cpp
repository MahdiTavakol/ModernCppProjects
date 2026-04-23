#include "output_settings.h"

output_settings::output_settings(int mode_):
	settings{mode_}
{
	this->type = outputType::SERIAL;
	this->outMode = outputMode::P3;
}

outputType& output_settings::return_type()
{
	return type;
}
outputMode& output_settings::return_outputMode()
{
	return outMode;
}

std::string output_settings::return_file_name();
{
	return file_name;
}

void output_settings::set_file_name(std::string file_name_)
{
	file_name = file_name_;
}

void output_settings::check_validity() const
{
	if (type == outputType::PARALLEL)
	{
		if (outMode != outputMode::P3)
			throw std::invalid_argument("Invalid output mode for PARALLEL output type! Only P3 is allowed.");
	}
}