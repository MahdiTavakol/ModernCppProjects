#include "Renderer_Profiler.hpp"

renderer_profiler::renderer_profiler(
	communicator* para_,
	Logger* error_,
	profiler* timer_,
	std::unique_ptr<path>&& pth_,
	std::string info_,
	bool verbose_) :
	renderer{ para_,error_,timer_,std::move(pth_),info_,verbose_ }
{}

renderer_profiler::~renderer_profiler()
{}

void renderer_profiler::set_parameters(int nProfilings_, std::unique_ptr<renderer>&& renderedObj_)
{
	nProfilings = nProfilings_;
	rendererObj = std::move(renderedObj_);
	
	for (int i = 0;i < nProfilings; i++)
	{
		timers.push_back(std::make_unique<profiler>(para, error));
	}
}

void renderer_profiler::render(image* img_, camera* cam_, output* writer_)
{
	for (auto& tmr : timers)
	{
		// resetting to timer to have an accurate run time
		tmr->reset_program_timer();
		tmr->start_event("rendering");
		rendererObj->render(img_, cam_, writer_);
		tmr->stop_event("rendering");
		tmr->start_event("Output");
		rendererObj->write_file(writer_, img_);
		tmr->stop_event("Output");
	}
	// the average timer
	std::unique_ptr<profiler> timer_avg = profiler::average_multiple_profilers(timers);
	timer_avg->print_timing_info();
}

void renderer_profiler::write_file(output* writer_, image* img_)
{
	// nothing to do here.
}