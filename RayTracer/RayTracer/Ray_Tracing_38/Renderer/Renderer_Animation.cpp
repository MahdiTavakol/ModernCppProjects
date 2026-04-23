#include "Renderer_Animation.h"
#include "../Output/output_parallel.h"



renderer_animation::renderer_animation(communicator *para_, settings* settingsObj_,std::string info_) :
	renderer{ para_, settingsObj_,info_}
{
	// The default value
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
		camera_settings* cam_settings = settingsObj->return_cam_settings();
		fps = cam_settings->get_fps();
		num_seconds = cam_settings->get_num_seconds();
		// Creating a new path variable containing the camera locations
		pth = std::make_unique<path>(center, radius, num_seconds, fps, theta);
		break;
	}
}

void renderer_animation::render(camera* cam_, output* writer_, hittable_list* world_)
{
	int rank = para->return_rank();

	int fps, num_frames, num_seconds, num_init_frames;
	pth->return_frame_info(fps, num_frames, num_seconds, num_init_frames);


	for (int i = 0; i < num_frames; i++)
	{
		message("Moving the camera to the frame " + std::to_string(i));
		cam_->move_camera((*pth)[i]);

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

