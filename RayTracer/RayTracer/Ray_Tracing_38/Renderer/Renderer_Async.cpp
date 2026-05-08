#include "Renderer_Async.h"
#include "../Algorithms/simpleComm.h"
#include "../Output/output_async.h"

renderer_async::renderer_async(
	communicator* para_,
	std::unique_ptr<path>&& pth_,
	std::unique_ptr<image_queue_fg>&& queue_,
	std::string info_,
	bool verbose_) :
	renderer{para_,std::move(pth_),info_,verbose_},
	 queue{std::move(queue)}
{
}


void renderer_async::render(camera* cam_, output* writer_, hittable_list* world_, material_list* list_)
{
	auto npos = writer_->return_binary_begin();

	path& pth_ref = *pth;
	cam_->move_camera(pth_ref[0]);

	auto render_lambda = [&cam_, &world_,&list_, &npos](image* img_thread_, output* out_thread_)
	{
		cam_->render(img_thread_,*world_,*list_);
		auto img = cam_->return_image();
		out_thread_->write_file(img.get(), npos);
	};

	while (!queue->empty())
	{
		std::unique_ptr<image> img_thread;
		std::unique_ptr<output> wrt_thread;
		queue->try_pop(img_thread, wrt_thread);
		std::thread thrd(render_lambda, img_thread.get(), wrt_thread.get());
	}
	std::string text = "Rendering the " + info;
	message(text);


	// since the image inside the camera is null due to the return_image,
	// the img needs to be returned to the camera so the camera is ready for
	// the next image... This matters for the animation mode
	//cam_->reset_image(std::move(img));
}


void renderer_async::write_file(output* writer_)
{
	std::string text = "Writing the data for " + info;
	message(text);

	// writing the file - it is the job of the writer to just write the file in the rank 0, so no need to check the rank here.
	writer_->write_file();
}




