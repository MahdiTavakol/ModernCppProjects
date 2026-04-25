#pragma once

#include "settings.h"
#include "../Types/vec3.h"

enum class Path_type
{
	NONE=0,
	CIRCULAR,
	FILE
};

class path_settings: public settings
{
public:
	path_settings(int mode);
	void set_input_map() override;

	// returning movie settings
	void return_movie_params(int& num_seconds_, int& fps_);

	// returning the type of the path
	void return_path_type(Path_type& pth_type_);
	// returning params for each path type
	void return_circular_params(point3& center_, double& radius_, double& theta_);
	void return_file_params(std::vector<point3>& centers_);

	void extra_parse() override;
	void check_validity() const override;
	void log_class_name(std::iostream& stream_) const override;

protected:
	// path type
	std::string pth_type_str;
	Path_type pth_type;
	// parameters for the path type circular
	// geometry
	point3 center;
	double radius;
	double theta;
	// parameters for the path type file
	// file containing the coordiates
	std::string file_name = "";
	std::vector<point3> centers;
	// movie properties
	int num_seconds = 0;
	int fps = 0;

};