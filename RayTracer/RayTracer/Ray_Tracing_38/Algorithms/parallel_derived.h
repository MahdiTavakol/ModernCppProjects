#ifndef PARALLEL_DERIVED_H
#define PARALLEL_DERIVED_H

#include "parallel.h"
#include "camera_derived.h"
#include "camera_parallel.h"

class parallel_derived : public parallel
{
public:
	parallel_derived();
	void initialize(std::unique_ptr<camera>& cam_, std::unique_ptr<hittable_list>& world_) override;
};

#endif