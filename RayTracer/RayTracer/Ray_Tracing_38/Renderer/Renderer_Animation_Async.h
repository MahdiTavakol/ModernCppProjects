#pragma once
#include "Renderer.h"

class renderer_animation_async : public renderer
{
public:
	renderer_animation_async(communicator* para_, std::unique_ptr<path>&& pth_, std::string info_ = "", bool verbose_ = false);
	void render(camera* cam_, output* writer_, hittable_list* world_) override;
	virtual void write_file(output* writer_) override
	{
		// the render function also writes the file!
	}

protected:
	int num_frames;
	void update_filename(std::string filename_);
};
