#include "camera_tiled.h"

camera_tiled::camera_tiled(std::unique_ptr<input>& _in, std::unique_ptr<parallel>& _para)
{
    _in->setup_camera(this);
    rank = _para->return_rank();
    size = _para->return_size();
}

camera_tiled::camera_tiled(const int _width_min, const int _width_max, const int _height_min, const int _height_max)
    : width_min(_width_min), width_max(_width_max), height_min(_height_min), height_max(_height_max) {
}

camera_tiled::camera_tiled()
    : width_min(0), width_max(0), height_min(0), height_max(0) {
}


void camera_tiled::render(const hittable& world)
{
    for (int j = height_min; j < height_max; j++)
    {
        for (int i = width_min; i < width_max; i++)
        {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; sample++)
            {
                ray r = get_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }
            color_data** c_data = c_array.return_array();
            pixel_color = pixel_samples_scale * pixel_color;
            c_data[i - width_min][j - height_min].r = pixel_color.x();
            c_data[i - width_min][j - height_min].g = pixel_color.y();
            c_data[i - width_min][j - height_min].b = pixel_color.z();
        }
    }
}

void camera_tiled::set_range(const int& _width_min, const int& _width_max, const int& _height_min, const int& _height_max)
{
    this->width_min = _width_min;
    this->width_max = _width_max;
    this->height_min = _height_min;
    this->height_max = _height_max;
}

void camera_tiled::initialize()
{
    camera::initialize();
    // setting up the camera
    int width_per_node = static_cast<int>(image_width / size) + 1;
    int height_per_node = image_height;
    width_min = rank * width_per_node;
    width_max = width_min + width_per_node;
    width_min = width_min >= image_width ? image_width : width_min;
    width_max = width_max >= image_width ? image_width : width_max;
    height_min = 0;
    height_max = image_height;
    set_range(width_min, width_max, height_min, height_max);
    // colors_array
    c_array.reset_size(width_per_node, height_per_node);
}