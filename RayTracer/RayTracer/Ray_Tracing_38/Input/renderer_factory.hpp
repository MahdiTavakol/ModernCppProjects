#pragma once
#include "../Renderer/Renderer.h"
#include "renderer_settings.h"
#include "../Algorithms/path.h"
#include "../Algorithms/communicator.h"
#include "../Algorithms/image_queue.h"

class renderer_factory
{
public:
	renderer_factory(settings* stngs_, communicator* para_,
		std::unique_ptr<image_queue_fg>&& que_ = nullptr
	);
	void create();
	std::unique_ptr<path> create_and_return_path();
	std::unique_ptr<renderer> return_object();

protected:
	std::unique_ptr<renderer> renderObj;
	communicator* para;

	// renderer_mode
	renderMode render_mode;

	std::unique_ptr<image_queue_fg> que;

	// movie settings
	int num_seconds = 0;
	int fps = 0;

	// path type
	Path_type pth_type = Path_type::NONE;

	// static path
	point3 location;

	//circular path
	point3 center;
	double radius;
	double theta;

	// general path 
	std::vector<point3_animated> centers;

};
