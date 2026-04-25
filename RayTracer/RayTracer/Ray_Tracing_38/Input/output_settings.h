#pragma once
#include "settings.h"
#include "../Shared/rtweekend.h"


class output_settings : public settings
{
public:
	output_settings(int mode_);
	void set_input_map() override;
	outputType return_type();
	outputMode return_outputMode();
	std::string return_file_name();
	void set_file_name(std::string file_name_);
	void check_validity() const override;
	void extra_parse() override;
	void parse_commands() override;
	void log_class_name(std::iostream& stream_) const;

private:
	std::string file_name;
	std::string output_type_str;
	std::string output_mode_str;
	outputType type;
	outputMode outMode;
};
