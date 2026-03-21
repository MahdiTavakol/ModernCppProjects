#include "parallel_derived.h"

parallel_derived::parallel_derived()
{}

void parallel_derived::initialize(std::unique_ptr<camera>& cam_, std::unique_ptr<hittable_list>& world_)
{
	parallel::initialize(cam_,world_);
	camera_derived* cam_type_test = dynamic_cast<camera_derived*>(cam_.get());
	hittable_list_parallel* world_type_test = dynamic_cast<hittable_list_parallel*>(world_.get());

	if (!cam_type_test) std::cerr << "Wrong camera type" << std::endl;
	if (!world_type_test) std::cerr << "Wrong world type" << std::endl;
}

