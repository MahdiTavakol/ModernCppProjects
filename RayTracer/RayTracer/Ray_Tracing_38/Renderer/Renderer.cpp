#include "Renderer.h"

renderer::renderer(communicator* para_, settings* settingsObj_, std::string info_, bool verbose_):
	info{info_},
	verbose{verbose_},
	para{ para_ },
	settingsObj{settingsObj_}
{}


renderer::renderer(int argc, char** argv, int _mode, std::string _filename,
	std::string obj_file_, std::array<int, 2> size_config_, MPI_Comm comm_):
	renderer{argc,argv,_mode,_filename, size_config_,comm_}
{
	obj_file_name = obj_file_;
}


renderer::~renderer()
{}



void renderer::render(camera* cam_, output* writer_, hittable_list* world_)
{
	std::string text = "Rendering the " + info;
	message(text);

	cam_->render(*world_);

	auto img = cam_->return_image();

	writer_->reset_image(filename, std::move(img));
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



