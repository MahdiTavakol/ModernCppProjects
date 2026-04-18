#ifndef CAMERA_PARALLEL_H
#define CAMERA_PARALLEL_H

#include "../Shared/rtweekend.h"

#include "camera.h"
#include "../Types/color.h"
#include "../Data/color_array.h"
#include "../Geometry/hittable.h"
#include "../Algorithms/parallel.h"
#include "../Input/input.h"
#include "../Types/vec3.h"


class camera_parallel : public camera
{
public:
    camera_parallel(
		std::unique_ptr<camera_settings>& _cam_setting,
        std::unique_ptr<parallel>& _para
    );




protected:

private:
   parallel* para;
};

#endif
