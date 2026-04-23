#pragma once
#include "settings.h"
#include "../Shared/rtweekend.h"


class output_settings : public settings
{
public:
	output_settings(int mode_);
	outputType& return_type();
	outputMode& return_outputMode();
	std::string return_file_name();
	void set_file_name(std::string file_name_);
	void check_validity() const override;

private:
	std::string file_name;
	outputType type;
	outputMode outMode;
};
