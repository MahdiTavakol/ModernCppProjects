#include "Renderer_Profiler.hpp"
#include "../Input/factory.hpp"

renderer_profiler::renderer_profiler(
	communicator* para_,
	Logger* error_,
	profiler* timer_,
	std::unique_ptr<path>&& pth_,
	std::string info_,
	bool verbose_) :
	renderer{ para_,error_,timer_,std::move(pth_),info_,verbose_ }
{
}

renderer_profiler::~renderer_profiler()
{}

void renderer_profiler::print_opening_message() const
{

}

void renderer_profiler::setup(hittable_list* world_,hittable_list* lights_, material_list* mtls_)
{
	// nothing here
}

void renderer_profiler::set_resource_pointers(
	std::vector<std::unique_ptr<profiler>>&& timers_,
	renderer_facade_inputs* ptrs_)
{
	nProfilings = timers_.size();
	timers = std::move(timers_);

	ptrs = ptrs_;
}

void renderer_profiler::render(image* img_, camera* cam_, output* writer_)
{
	for (auto& tmr : timers)
	{
		int event_number = static_cast<int>(&tmr - timers.data());
		error->print_message(std::string(print_len, '-'), msg_level);
		error->print_message("Profiling event " + std::to_string(event_number), msg_level);
		error->print_message(std::string(print_len, '.'), msg_level);
		error->print_message("\tBuilding resources", msg_level);
		// resetting to timer to have an accurate run time
		tmr->reset_program_timer();
		// the factory object
		auto& argv_vec = *(ptrs->argv_vec);
		int mode = ptrs->mode;
		MPI_Comm comm = ptrs->comm;
		// we have send the timer and error objects to the builder object
		// so it itself record its own individual timings
 		auto builder = std::make_unique<factory>(
			argv_vec, mode, comm,error,tmr.get());
		// creating the objects
		builder->create();
		// returning each object
		auto rend = builder->return_renderer();
		auto cam = builder->return_camera();
		auto para = builder->return_comm();
		auto writer = builder->return_writer();
		auto world = builder->return_world();
		auto lights = builder->return_lights();
		auto mtl_list = builder->return_mtl_list();
		auto img = builder->return_image();
		// objects with specific setup methods
		writer->setup(img.get());
		rend->setup(world.get(), lights.get(), mtl_list.get());



		error->print_message("\tRendering", msg_level);
		tmr->start_event("rendering");
		rend->render(img.get(), cam.get(), writer.get());
		tmr->stop_event("rendering");
		error->print_message("\tOutput", msg_level);
		tmr->start_event("Output");
		rend->write_file(writer.get(), img.get());
		tmr->stop_event("Output");
		// stopping the program counter
		tmr->stop_program_counter();
	}
	error->print_message(std::string(print_len, '='),msg_level);
	error->print_message("Finished profiling", msg_level);
	error->print_message(std::string(print_len, '.'), msg_level);
}

void renderer_profiler::write_file(output* writer_, image* img_)
{
	// nothing to do here.
}

void renderer_profiler::print_timing_info()
{
	error->print_message(std::string(print_len, '.'), msg_level);
	error->print_message("Timing information", msg_level);
	error->print_message(std::string(print_len, '='), msg_level);
	// the average timer
	std::unique_ptr<profiler> timer_avg = profiler::average_multiple_profilers(timers);
	timer_avg->print_timing_info(print_len);

}