#include "Renderer.h"

renderer::renderer(communicator* para_,
	Logger* error_,
	profiler* timer_,
	std::unique_ptr<path>&& pth_,
	std::string info_, 
	bool verbose_):
	info{info_},
	verbose{verbose_},
	para{ para_ },
	error{error_},
	timer{timer_},
	pth{std::move(pth_)}
{
}


renderer::~renderer()
{}


void renderer::setup(hittable_list* world_, material_list* mtls_)
{
	world = world_;
	mtls = mtls_;
}


void renderer::render(image* img_, camera* cam_, output* writer_)
{
	std::string text = std::string(52, '=');
	int msg_level = 0;
	error->print_message(text, msg_level);
	text = "Rendering the " + info;
	msg_level = 0;
	error->print_message(text, msg_level);
	text = std::string(52, '=');
	msg_level = 0;
	error->print_message(text, msg_level);

	std::string event = "Moving camera";
	timer->start_event(event);
	path& pth_ref = *pth;
	cam_->move_camera(pth_ref[0]);
	timer->stop_event(event);

	event = "Rendering";
	timer->start_event(event);
	cam_->render(img_,*world,*mtls);
	timer->stop_event(event);


	// since the image inside the camera is null due to the return_image,
	// the img needs to be returned to the camera so the camera is ready for
	// the next image... This matters for the animation mode
	//cam_->reset_image(std::move(img));

}


void renderer::write_file(output* writer_, image* img_)
{
	std::string text = std::string(52, '=');
	int msg_level = 0;
	error->print_message(text, msg_level);
	text = "Writing the image for " + info;
	msg_level = 0;
	error->print_message(text, msg_level);
	text = std::string(52, '=');
	error->print_message(text, msg_level);

	std::string event = "Writing the image";
	timer->start_event(event);
	// writing the file - it is the job of the writer to just write the file in the rank 0, so no need to check the rank here.
	writer_->write_file(img_);
	timer->stop_event(event);
}




