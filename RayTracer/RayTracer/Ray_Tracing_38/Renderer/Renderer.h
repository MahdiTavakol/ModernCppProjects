#ifndef RENDERER_H
#define RENDERER_H
#include <memory>

#include "../Shared/rtweekend.h"

#include "../Input/obj_model_reader.h"
#include "../Input/obj_model_reader_parallel.h"
#include "../Geometry/aabb.h"
#include "../Algorithms/bvh.h"
#include "../Algorithms/camera.h"
#include "../Algorithms/camera_settings.h"
#include "../Input/scene_factory.h"
#include "../Algorithms/camera.h"
#include "../Algorithms/image.h"
#include "../Types/color.h"
#include "../Data/color_array.h"
#include "../Geometry/constant_medium.h"
#include "../Geometry/hittable.h"
#include "../Algorithms/hittable_list.h"
#include "../Input/input.h"
#include "../Geometry/interval.h"
#include "../Materials/material.h"
#include "../Algorithms/path.h"
#include "../Algorithms/communicator.h"
#include "../Algorithms/mpiComm.h"
#include "../Geometry/quad.h"
#include "../Types/ray.h"
#include "../Algorithms/rtw_stb_image.h"
#include "../Geometry/planes.h"
#include "../Geometry/perlin.h"
#include "../Geometry/sphere.h"
#include "../Geometry/texture.h"
#include "../Types/vec3.h"
#include "../Output/output.h"
#include "../Output/output_serial.h"


class renderer
{
public:
	renderer(communicator* para_,std::unique_ptr<path>&& pth_, std::string info_ = "", bool verbose_ = false);

	virtual ~renderer();
	virtual void render(camera* cam_, output* writer_, hittable_list* world_);
	void write_file(output* writer_);
	
	

protected:
	std::string info = "";
	bool verbose = false;
	communicator* para;

	std::unique_ptr<path> pth;

	void message(std::string _text);
	std::string filename;
};

#endif
