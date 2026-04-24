#pragma once

#include "settings.h"
#include "../Types/vec3.h"

class path_settings: public settings
{
public:
	path_settings(int mode);
	void set_input_map() override;
	void return_movie_params(int& num_seconds_, int& fps_);
	void return_geom_params(point3& center_, double& radius_, double& theta_);

protected:
	// geometry
	point3 center;
	double radius;
	double theta;
	// movie properties
	int num_seconds = 0;
	int fps = 0;
	// file containing the coordiates
	std::string file_name;
	std::fstream file;

};