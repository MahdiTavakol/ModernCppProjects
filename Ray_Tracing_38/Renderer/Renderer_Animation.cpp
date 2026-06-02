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


void renderer_animation::render(image* img_, camera* cam_, output* writer_)
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
		cam_->render(img_,*world,*mtls);

		writer_->reset_filename(filename);
		writer_->setup(img_);

		text = "Writing the frame " + std::to_string(i);
		message(text);

		// writing the file - it is the job of the writer to just write the file in the rank 0, so no need to check the rank here.
		writer_->write_file(img_);
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

void renderer_animation::write_file(output* writer_, image* img_)
{

}

