#include "Renderer_Animation.h"
#include "../Output/output_parallel.h"



renderer_animation::renderer_animation(communicator* para_, std::unique_ptr<path>&& pth_, std::string info_, bool verbose_) :
	renderer{ para_, std::move(pth_),info_,verbose_}
{
	int rank = para->return_rank();
	if (rank != 0)
		verbose = false;

	num_frames = pth->return_num_frames();
}


void renderer_animation::render(camera* cam_, output* writer_, hittable_list* world_)
{
	// getting a reference to the pth resource
	path& pth_ref = *pth;


	for (int i = 0; i < num_frames; i++)
	{
		message("Moving the camera to the frame " + std::to_string(i));
		cam_->move_camera(pth_ref[i]);

		message("Updating the filename for the frame " + std::to_string(i));
		info = "frame-" + std::to_string(i);
		update_filename(info);

		std::string text = "Rendering the frame " + std::to_string(i);
		message(text);

		// since the image inside the camera is null due to the return_image,
		// the img needs to be returned to the camera so the camera is ready for
		// the next image...

		std::unique_ptr<image> img1 = writer_->return_image();
		cam_->reset_image(std::move(img1));
		cam_->render(*world_);


		text = "Writing the frame " + std::to_string(i);
		message(text);

		std::unique_ptr<image> img2 = cam_->return_image();
		writer_->reset_image(std::move(img2));
	}
}

void renderer_animation::render_async(camera* cam_, output* writer_, hittable_list* world_)
{
	// getting a reference to the pth resource
	path& pth_ref = *pth;


	for (int i = 0; i < num_frames; i++)
	{
		message("Moving the camera to the frame " + std::to_string(i));
		cam_->move_camera(pth_ref[i]);

		message("Updating the filename for the frame " + std::to_string(i));
		info = "frame-" + std::to_string(i);
		update_filename(info);

		std::string text = "Rendering the frame " + std::to_string(i);
		message(text);

		// since the image inside the camera is null due to the return_image,
		// the img needs to be returned to the camera so the camera is ready for
		// the next image...

		std::unique_ptr<image> img1 = writer_->return_image();
		cam_->reset_image(std::move(img1));
		cam_->render(*world_);


		text = "Writing the frame " + std::to_string(i);
		message(text);

		std::unique_ptr<image> img2 = cam_->return_image();
		writer_->reset_image(std::move(img2));
	}
}

void renderer_animation::update_filename(std::string filename_)
{
#ifdef _WIN32
	filename = "temp\\" + info + ".ppm";
#else
	filename = "temp/" + info + ".ppm";
#endif
}

