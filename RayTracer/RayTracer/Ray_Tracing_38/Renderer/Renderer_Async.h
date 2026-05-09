#pragma once
#include "Renderer.h"
#include "../Algorithms/image_queue.h"
#include <thread>

class renderer_async : public renderer
{
public:
	renderer_async(
		communicator* para_,
		std::unique_ptr<path>&& pth_,
		int max_threads_,
		std::unique_ptr<image_queue_fg>&& queue_,
		std::string info_ = "",
		bool verbose_ = false);

	virtual void render(camera* cam_, output* writer_, hittable_list* world_, material_list* list_) override;
	virtual void write_file(output* writer_) override;


private:
	std::once_flag queue_flag;
	std::unique_ptr<image_queue_fg> queue;
	int max_threads;

	void render_thread(camera& cam_,
		               const hittable_list& world_, 
		               const material_list& list_, 
		               const std::streampos npos_);
};