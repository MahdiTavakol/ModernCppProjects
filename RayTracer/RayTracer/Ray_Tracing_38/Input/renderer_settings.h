#pragma once

#include "settings.h"
#include "../Types/vec3.h"
#include "../Types/point3_animated.h"

enum class Path_type
{
	NONE=0,
	CIRCULAR,
	FILE
};

class renderer_settings: public settings
{
public:
	renderer_settings(int mode_, bool wrong_access_= false);
	void set_input_map() override;
	void set_mode(int mode_) override;
	void extra_parse() override;
	void check_validity() const override;
	void log_class_name(std::iostream& stream_) const override;

	// returning the render types
	void return_render_mode(renderMode& render_mode_) const
	{
		render_mode_ = render_mode;
	}

	// returning movie settings
	void return_movie_params(int& num_seconds_, int& fps_);
	// returning the type of the path
	void return_path_type(Path_type& pth_type_);
	// returning params for each path type
	void return_location_param(point3& location_);
	void return_circular_params(point3& center_, double& radius_, double& theta_);
	void return_file_params(std::vector<point3_animated>& centers_);
	void return_file_name(std::string& file_name_) { file_name_ = file_name; }



protected:
	bool wrong_access = false;
	// renderer type
	std::string renderer_mode_str = "STATIC";
	renderMode render_mode = renderMode::STATIC;
	// path type
	std::string pth_type_str = "NONE";
	Path_type pth_type = Path_type::NONE;
	// parameters for the path type none (static)
	point3 location = point3(13.0,2.0,3.0);
	// parameters for the path type circular
	// geometry
	point3 center;
	double radius;
	double theta;
	// parameters for the path type file
	// file containing the coordiates
	std::string file_name = "";
	std::vector<point3_animated> centers;
	// movie properties
	int num_seconds = 0;
	int fps = 0;

};