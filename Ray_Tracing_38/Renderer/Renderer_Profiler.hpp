#pragma once
#include "Renderer.h"
#include <mpi.h>


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
	virtual void print_opening_message() const override;
	void setup(hittable_list* world_, hittable_list* lights_, material_list* mtls_) override;
	void set_resource_pointers(
		std::vector<std::unique_ptr<profiler>>&& timers_,
		renderer_facade_inputs* ptrs_) override;
	virtual void render(image* img_, camera* cam_, output* writer_) override;
	virtual void write_file(output* writer_, image* img_) override;
	void print_timing_info() override;

protected:
	size_t print_len = 82;
	std::vector<std::unique_ptr<profiler>> timers;
	renderer_facade_inputs* ptrs;
	int nProfilings = 10;
	const int msg_level = -2;

};