#pragma once
#include "Renderer.h"

class renderer_profiler : public renderer
{
public:
	renderer_profiler(
		communicator* para_,
		Logger* error_,
		profiler* timer_,
		std::unique_ptr<path>&& pth_,
		std::string info_ = "",
		bool verbose_ = false);
	virtual ~renderer_profiler() override;
	void set_parameters(int nProfilings_, std::unique_ptr<renderer>&& renderedObj_);
	virtual void render(image* img_, camera* cam_, output* writer_) override;
	virtual void write_file(output* writer_, image* img_) override;

protected:
	std::unique_ptr<renderer> rendererObj;
	std::vector<std::unique_ptr<profiler>> timers;
	int nProfilings = 10;

};