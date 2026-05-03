#include "Renderer_Animation_Async.h"
#include "../Output/output_parallel.h"



renderer_animation_async::renderer_animation_async(
	communicator* para_, 
	std::unique_ptr<path>&& pth_,
	std::string info_,
	bool verbose_) :
	renderer{ para_, std::move(pth_),info_,verbose_ }
{
	int rank = para->return_rank();
	if (rank != 0)
		verbose = false;

	num_frames = pth->return_num_frames();
}

void renderer_animation_async::render(camera* cam_, output* writer_, hittable_list* world_)
{
	// getting a reference to the pth resource
	path& pth_ref = *pth;


	image* buffer1 = cam_->return_image_ptr();
	image* buffer2 = writer_->return_image_ptr();

	std::unique_ptr<std::thread> writer_thread;
	std::unique_ptr<std::thread> cam_thread;

	for (int i = 0; i < num_frames; i++)
	{
		message("Moving the camera to the frame " + std::to_string(i));
		cam_->move_camera(pth_ref[i]);

		message("Updating the filename for the frame " + std::to_string(i));
		info = "frame-" + std::to_string(i);
		update_filename(info);

		std::string text = "Rendering the frame " + std::to_string(i);
		message(text);

		if (cam_thread)
		{
			cam_thread->join();
		}
		if (writer_thread)
		{
			writer_thread->join();
			std::swap(buffer1, buffer2);
		}

		//cam_->render(*world_);
		cam_thread = std::make_unique<std::thread>(&camera::render_async, cam_, std::ref(*world_), buffer1);


		text = "Writing the frame " + std::to_string(i);
		message(text);

		// writing the file - it is the job of the writer to just write the file in the rank 0, so no need to check the rank here.
		//writer_->write_file();
		if (i)
		{
			writer_thread = std::make_unique<std::thread>(&output::write_file_async, writer_, filename, buffer2);
		}
	}

	if (writer_thread && writer_thread->joinable())
		writer_thread->join();
	if (cam_thread && cam_thread->joinable())
		cam_thread->join();

}

void renderer_animation_async::update_filename(std::string filename_)
{
#ifdef _WIN32
	filename = "temp\\" + info + ".ppm";
#else
	filename = "temp/" + info + ".ppm";
#endif
}

void renderer_animation_async::write_file(output* writer_)
{

}

