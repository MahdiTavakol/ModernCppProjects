#include "Renderer_Animation.h"
#include "../Output/output_parallel.h"



renderer_animation::renderer_animation(communicator* para_, std::unique_ptr<path>&& pth_, std::string info_, bool verbose_) :
	renderer{ para_, std::move(pth_),info_,verbose_}
{
	int rank = para->return_rank();
	if (rank != 0)
		verbose = false;
}


void renderer_animation::render(camera* cam_, output* writer_, hittable_list* world_)
{
	int rank = para->return_rank();

	int fps, num_frames, num_seconds, num_init_frames;
	pth->return_frame_info(fps, num_frames, num_seconds, num_init_frames);

	// getting a reference to the pth resource
	path& pth_ref = *pth;


	for (int i = 0; i < num_frames; i++)
	{
		message("Moving the camera to the frame " + std::to_string(i));
		cam_->move_camera(pth_ref[i]);

		message("Updating the filename for the frame" + std::to_string(i));
		info = "frame-" + std::to_string(i);
		update_filename(info);

		renderer::render(cam_, writer_, world_);
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

