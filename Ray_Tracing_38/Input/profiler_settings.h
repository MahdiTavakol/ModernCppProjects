#pragma once

#include "settings.h"
#include <array>

class profiler_settings : public settings
{
public:
	profiler_settings(int mode_);
	void set_input_map() override;
	void check_validity() const override
	{
		// empty for now! 
	}
	void return_profiling_info(bool& profiling_, int& nProfilings_);
	void log_class_name(std::iostream& stream_) const override;

protected:
	int nProfilings = 1;
};