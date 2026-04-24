#pragma once

#include "settings.h"
#include <array>

class communicator_settings : public settings
{
public:
	communicator_settings();
	void set_input_map() override;
	void check_validity() const override
	{
		// empty for now! 
	}
	void return_size_config(std::array<int, 2>& size_config_);

protected:
	std::array<int, 2> size_config = { 1,1 };
};