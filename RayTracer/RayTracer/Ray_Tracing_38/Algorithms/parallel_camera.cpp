#include "parallel_camera.h"

parallel_camera::parallel_camera():
    width_per_node{ 0 }, height_per_node{0},
    width_min{0}, width_max{0}, height_min{0}, height_max{0}
{
}

void parallel_camera::initialize(std::unique_ptr<camera>& cam_, [[maybe_unused]] std::unique_ptr<hittable_list>& world_)
{
    // pointing the c_array to the color_array object inside the camera
    parallel::initialize(cam_,world_);
    // setting up the camera
    width_per_node = static_cast<int>(cam_->image_width / size) + 1;
    height_per_node = cam_->image_height;
    width_min = rank * width_per_node;
    width_max = width_min + width_per_node;
    width_min = width_min >= cam_->image_width ? cam_->image_width : width_min;
    width_max = width_max >= cam_->image_width ? cam_->image_width : width_max;
    height_min = 0;
    height_max = cam_->image_height;
    cam_->set_range(width_min, width_max, height_min, height_max);
    // colors_array
    c_array->reset_size(width_per_node, height_per_node);
}


void parallel_camera::gather(std::unique_ptr<camera>& cam)
{
    color_data* colors = c_array->return_array()[0];
    color_data* colors_all = (color_data*)malloc(width_per_node * height_per_node * size * sizeof(color_data));

    int num_double_data = width_per_node * height_per_node * sizeof(color_data) / sizeof(double);

    MPI_Allgather(colors, num_double_data, MPI_DOUBLE, colors_all, num_double_data, MPI_DOUBLE, MPI_world);
    // a copy of data is involved here I should work on removing this!
    c_array_all = std::make_unique<color_array>(cam->image_width, cam->image_height, colors_all);
    free(colors_all);
    colors_all = nullptr;
}

color_array* parallel_camera::return_color_array_ptr()
{
    return c_array_all.get();
}