#pragma once
#include "Renderer.h"
#include "../Algorithms/image_async.h"
#include "../Output/output_async.h"
#include <thread>

class renderer_async : public renderer
{
public:
	renderer_async(
		communicator* para_,
		std::unique_ptr<path>&& pth_,
		int max_threads_,
		std::string info_ = "",
		bool verbose_ = false);

	virtual void render(camera* cam_, output* writer_, hittable_list* world_, material_list* list_) override;
	virtual void write_file(output* writer_) override;


private:
	int max_threads;

	void render_thread(camera* cam_,
		               hittable_list* world_, 
		               material_list* list_,
					   image_async* img_async_,
					   output_async* out_async_,
		               const std::streampos npos_);
};