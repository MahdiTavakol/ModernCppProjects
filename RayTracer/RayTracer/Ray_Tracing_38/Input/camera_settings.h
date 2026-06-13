#pragma once

#include "../Types/vec3.h"
#include "../Types/color.h"
#include "settings.h"
#include "../Shared/rtweekend.h"


class camera_settings: public settings
{
public:
	camera_settings() = delete;
	camera_settings(int mode_);
	void set_input_map() override;
	void check_validity() const override;
	void log_class_name(std::iostream& stream_) const;

	void set_mode(int mode_) override;
	bool set_from_scene(hittable_list& world_) override;
	int& get_samples_per_pixel();
	int& get_max_depth();
	int& get_vfov();
	double& get_defocus_angle();
	double& get_focus_dist();
	point3& get_lookat();
	point3& get_vup();
	color& get_background();
	bool get_stratified();
	std::string return_HDRI_name();
	void extra_parse() override;

protected:

	int samples_per_pixel = 150;
	int max_depth = 50;
	int vfov = 45;
	double defocus_angle = 0.6;
	double focus_dist = 10.0;
		
	point3 lookat = point3(0, 0, 0);
	point3 vup = point3(0, 1, 0);


	color background = color(0.7, 0.8, 1.00);
	std::string HDRI_name = "";


	std::string stratified_string = "off";
	bool  stratified = false;


};