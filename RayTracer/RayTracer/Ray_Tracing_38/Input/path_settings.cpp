#include "path_settings.h"

path_settings::path_settings(int mode_):
	settings{mode_}
{}

void path_settings::set_input_map()
{
	singleInputMap = {
		{"-radius",&radius},
		{"-theta",&theta},
		{"-num_seconds",&num_seconds},
		{"-fps",&fps}
	};
}

void path_settings::return_movie_params(int& num_seconds_, int& fps_)
{
	num_seconds = num_seconds_;
	fps = fps_;
}

void path_settings::return_geom_params(point3& center_, double& radius_, double& theta_)
{
	center_ = center;
	radius_ = radius;
	theta_ = theta;
}