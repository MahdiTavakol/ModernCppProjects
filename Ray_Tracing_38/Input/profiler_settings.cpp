#include "profiler_settings.h"

profiler_settings::profiler_settings(int mode_):
	settings{mode_},
	nProfilings{1}
{
	set_input_map();
}

void profiler_settings::set_input_map()
{
	singleInputMap = {
		{"-nprofiles",&nProfilings}
	};
}

void profiler_settings::return_profiling_info(bool& profiling_, int& nProfilings_)
{
	if (nProfilings > 1)
	{
		profiling_ = true;
	}
	else
	{
		profiling_ = false;
	}
	nProfilings_ = nProfilings;
}

void profiler_settings::log_class_name(std::iostream& stream_) const
{
	stream_ << "Profiler class options:" << std::endl << std::endl;
}