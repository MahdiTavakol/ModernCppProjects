#include "communicator_settings.h"

communicator_settings::communicator_settings()
{
	set_input_map();
}

void communicator_settings::set_input_map()
{
	doubleInputMap = {
		{"-size_config",std::pair{&size_config[0],&size_config[1]}}
	};
}

void communicator_settings::return_size_config(std::array<int, 2>& size_config_)
{
	size_config_ = size_config;
}