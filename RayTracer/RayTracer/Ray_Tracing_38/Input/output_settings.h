#pragma once
#include "settings.h"
#include "../Shared/rtweekend.h"
#include <array>


class output_settings : public settings
{
public:
	output_settings(int mode_);
	void set_input_map() override;
	outputType return_type();
	outputMode return_outputMode();
	std::string return_file_name();
	void set_mode(int mode_) override;
	std::array<int, 2> return_size() const;
	void set_file_name(std::string file_name_);
	void check_validity() const override;
	void extra_parse() override;
	void parse_commands() override;
	void log_class_name(std::iostream& stream_) const;
	std::string get_file_name() const { return file_name; }

private:
	std::string file_name;
	std::string output_type_str = "SERIAL";
	std::string output_mode_str = "P3";
	std::array<int, 2> size;
	outputType type;
	outputMode outMode;
};
