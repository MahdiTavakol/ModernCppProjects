#include "Renderer_Animation.h"
#include "../Output/output_parallel.h"



renderer_animation::renderer_animation(int argc, char** argv, int _mode, [[maybe_unused]] std::string _file_name, MPI_Comm comm_) :
	renderer{ argc,argv,_mode,"",std::array<int,2>{1,1}, comm_ }
{
	// The default value
	// mode = RANDOM_SPHERES_ANIMATED;
	pth = std::make_unique<path>(point3(0, 0, 12));
}

renderer_animation::renderer_animation(int argc, char** argv, int _mode, std::string _filename,
	std::string obj_file_, MPI_Comm comm_) :
	renderer_animation{ argc,argv,_mode,_filename, comm_ }
{
	obj_file_name = obj_file_;
}

void renderer_animation::setup()
{
	renderer::setup();
	switch (mode)
	{
	case RANDOM_SPHERES_ANIMATED:
		// The movie setting
		int fps, num_seconds;
		// Creating the trajectory of the camera
		point3 center(13, 2, 3);
		double radius = 13.49;
		double theta = 45.0;
		// Get the fps and num_frames
		fps = cam_settings->get_fps();
		num_seconds = cam_settings->get_num_seconds();
		// Creating a new path variable containing the camera locations
		pth = std::make_unique<path>(center, radius, num_seconds, fps, theta);
		break;
	}
}

void renderer_animation::render()
{
	int rank = para->return_rank();

	int fps, num_frames, num_seconds, num_init_frames;
	pth->return_frame_info(fps, num_frames, num_seconds, num_init_frames);


	for (int i = 0; i < num_frames; i++)
	{
		message(rank, "Moving the camera to the frame " + std::to_string(i));

		move_camera(i);

		message(rank, "Rendering the frame " + std::to_string(i) + "                ");

		renderer::render();

		message(rank, "Gathering data from nodes for frame " + std::to_string(i));


		if (rank == 0)
		message(rank, "Opening the file for the frame" + std::to_string(i));
		if (rank == 0)
			open_file(i);

		message(rank, "Writing the data for frame " + std::to_string(i) + "     ");
		if (rank == 0)
			write_file();

		message(rank, "Closing the current file");		

	}
}


void renderer_animation::open_file(const int frame_i)
{
	int fps, num_seconds, num_frames;
	fps = cam_settings->get_fps();
	num_seconds = cam_settings->get_num_seconds();
	num_frames = fps * num_seconds;
#ifdef _WIN32
	std::string file_name = "temp\\frame-" + std::to_string(frame_i) + ".ppm";
#else
	std::string file_name = "temp/frame-" + std::to_string(frame_i) + ".ppm";
#endif
	// getting the image from the camera.
	auto img = cam->return_image();
	// the only resource that output_parallel has
	// is the image which needs to be replaced for each frame.
	// So creating a new output_parallel object here
	// does not cause extra overhead.
	writer = std::make_unique<output_parallel>(file_name, std::move(img), para, outputMode::P6);
}

int renderer_animation::return_num_frames() const
{
	int fps, num_seconds, num_frames;
	fps = cam_settings->get_fps();
	num_seconds = cam_settings->get_num_seconds();
	num_frames = fps * num_seconds;
	return num_frames;
}

void renderer_animation::move_camera(const int frame_i)
{
	// moving the camera
	cam->move_camera((*pth)[frame_i]);
}

void renderer_animation::message(int _rank, std::string _text)
{
	if (_rank == 0)
	{
		std::clog << _text << std::endl;
		fflush(stdout);
	}
}
