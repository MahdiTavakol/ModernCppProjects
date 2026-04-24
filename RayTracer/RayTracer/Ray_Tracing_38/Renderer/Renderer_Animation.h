#pragma once
#include "Renderer.h"

class renderer_animation final : public renderer
{
public:
	renderer_animation(communicator *para_, settings* settingsObj_, std::string info_);
	void setup(camera* cam_) override;
	void render(camera* cam_, output* writer_, hittable_list* world_) override;


private:
	std::unique_ptr<path> pth;
	void update_filename(std::string filename_);
};