#pragma once
#include "settings.h"


class image_settings: public settings
{
public:
	image_settings(int mode_);

	void set_mode(int mode_) override;

	int& get_image_width();
	int get_image_height();
	double& get_width_ratio();
	double& get_height_ratio();

public:
	int image_width = 1080;
	double width_ratio = 16.0;
	double height_ratio = 9.0;
};