#include "camera_parallel.h"
#include "image_parallel.h"

#include <mpi.h>

camera_parallel::camera_parallel(
    std::unique_ptr<camera_settings>& cam_setting_, std::unique_ptr<parallel>& _para) :
    camera{cam_setting_},
    para{ _para.get() }
{
    img = std::make_unique<image_parallel>(cam_setting_, _para);
}






