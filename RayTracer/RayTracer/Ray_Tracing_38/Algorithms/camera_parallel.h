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
    camera_parallel(std::unique_ptr<input>& _in, std::unique_ptr<parallel>& para);
    camera_parallel(const int _width_min, const int _width_max, const int _height_min, const int _height_max);
    camera_parallel();
    void render(const hittable& world) override;
    void set_range(const int& _width_min, const int& _width_max, const int& _height_min, const int& _height_max) override;

private:
    int height_min, height_max, width_min, width_max;
    int rank, size;
};

#endif
