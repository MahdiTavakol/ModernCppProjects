#include "camera_parallel.h"

#include <mpi.h>

camera_parallel::camera_parallel(
    std::unique_ptr<input>& _in, std::unique_ptr<parallel>& _para):
    camera{}
{
    _in->setup_camera(this);
    rank = _para->return_rank();
    size = _para->return_size();
    world = _para->return_comm();
    rank_config = _para->return_rank_config();
    size_config = _para->return_size_config();

    // setting up the camera
    auto set_range_per_node = [&](const int dimension,const int myRank,const int mySize,
                                  int& min,int& max, int& perNode) {
            perNode = static_cast<int>(dimension / mySize) + 1;
            min = myRank * perNode;
            max = min + perNode;
            min = min >= dimension ? dimension : min;
            max = max >= dimension ? dimension : max;
           
        };

    set_range_per_node(image_width, rank_config[0], size_config[0],
        width_min, width_max, width_per_node);
    set_range_per_node(image_height, rank_config[1], size_config[1],
        height_min, height_max, height_per_node);
    
    set_range(width_min, width_max, height_min, height_max);

    initialize_storage();
}

camera_parallel::camera_parallel(
    const int _width_min, const int _width_max, 
    const int _height_min, const int _height_max)
    : width_min(_width_min), width_max(_width_max), height_min(_height_min), height_max(_height_max) {}

camera_parallel::camera_parallel()
    : width_min(0), width_max(0), height_min(0), height_max(0) {}


void camera_parallel::render(const hittable& world)
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
            color_data** c_data = c_array->return_array();
            pixel_color = pixel_samples_scale * pixel_color;
            c_data[i - width_min][j - height_min].r = pixel_color.x();
            c_data[i - width_min][j - height_min].g = pixel_color.y();
            c_data[i - width_min][j - height_min].b = pixel_color.z();
        }
    }
}

void camera_parallel::set_range(const int& _width_min, const int& _width_max, const int& _height_min, const int& _height_max)
{
    this->width_min = _width_min;
    this->width_max = _width_max;
    this->height_min = _height_min;
    this->height_max = _height_max;
}

color_array* camera_parallel::return_color_array_ptr()
{
    gather();
    return camera::return_color_array_ptr();
}

void camera_parallel::gather()
{
    color_data* colors = c_array->return_array()[0];
    color_data* colors_all = (color_data*)malloc(width_per_node * height_per_node * size * sizeof(color_data));

    int num_double_data = width_per_node * height_per_node * sizeof(color_data) / sizeof(double);

    // this part might be moved to the parallel class to generalize differnt gather strategies
    MPI_Allgather(colors, num_double_data, MPI_DOUBLE, colors_all, num_double_data, MPI_DOUBLE, *world);
    // a copy of data is involved here I should work on removing this!
    c_array_all = std::make_unique<color_array>(image_width, image_height, colors_all);
    free(colors_all);
    colors_all = nullptr;
    // swapping 
    c_array.swap(c_array_all);
}

void camera_parallel::initialize_storage()
{
    // colors_array
    c_array = std::make_unique<color_array>(width_per_node, height_per_node);
    c_array_all = std::make_unique<color_array>(image_width, image_height);
}
