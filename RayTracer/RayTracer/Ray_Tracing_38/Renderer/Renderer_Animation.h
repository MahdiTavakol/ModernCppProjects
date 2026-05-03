#pragma once
#include "Renderer.h"

class renderer_animation final : public renderer
{
public:
	renderer_animation(communicator* para_, std::unique_ptr<path>&& pth_, std::string info_ = "", bool verbose_ = false);
	void render(camera* cam_, output* writer_, hittable_list* world_) override;
	void render_async(camera* cam_, output* writer_, hittable_list* world_);


private:
	int num_frames;
	void update_filename(std::string filename_);

};