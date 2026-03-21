#ifndef PARALLEL_CAMERA_H
#define PARALLEL_CAMERA_H

#include "parallel.h"

class parallel_camera : public parallel
{
public:
	parallel_camera();
	void initialize(std::unique_ptr<camera>& cam_, std::unique_ptr<hittable_list>& world_) override;
	void gather(std::unique_ptr<camera>& cam) override;
	color_array* return_color_array_ptr() override;
protected:
	int width_per_node, height_per_node;
	int width_min, width_max, height_min, height_max;

private:
	// the parallel owns the c_array_all despite the c_array
	// since the parallel is in charge of combining the c_arrays
	// from all the camera
	std::unique_ptr<color_array> c_array_all;
	
};



#endif