#include "Renderer.h"

renderer::renderer(communicator* para_, std::unique_ptr<path>&& pth_, std::string info_, bool verbose_):
	info{info_},
	verbose{verbose_},
	para{ para_ }
{
}


renderer::~renderer()
{}



void renderer::render(camera* cam_, output* writer_, hittable_list* world_)
{
	std::string text = "Rendering the " + info;
	message(text);


	path& pth_ref = *pth;
	cam_->move_camera(pth_ref[0]);

	cam_->render(*world_);

	auto img = cam_->return_image();

	writer_->reset_image(filename, std::move(img));

	// since the image inside the camera is null due to the return_image,
	// the img needs to be returned to the camera so the camera is ready for
	// the next image... This matters for the animation mode
	cam_->reset_image(std::move(img));
}


void renderer::write_file(output* writer_)
{
	std::string text = "Writing the data for " + info;
	message(text);

	// writing the file - it is the job of the writer to just write the file in the rank 0, so no need to check the rank here.
	writer_->write_file();
}

void renderer::message(std::string _text)
{
	if (verbose)
	{
		int rank = para->return_rank();
		if (rank == 0)
		{
			std::clog << _text << std::endl;
			fflush(stdout);
		}
	}
}



