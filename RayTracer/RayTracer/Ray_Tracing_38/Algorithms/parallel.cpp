#include "parallel.h"

parallel::parallel()
{
    // MPI variables
    MPI_world = MPI_COMM_WORLD;
    MPI_Comm_rank(MPI_world, &rank);
    MPI_Comm_size(MPI_world, &size);
}

parallel::~parallel()
{
}

void parallel::initialize(std::unique_ptr<camera>& cam_, std::unique_ptr<hittable_list>& world_)
{
    // getting the color_array from the camera
    c_array = cam_->return_color_array_ptr();
}

color_array* parallel::return_color_array_ptr()
{
    return c_array;
}

int parallel::return_rank() const {
    return rank;
}

int parallel::return_size() const {
    return size;
}

void parallel::barrier() const {
    MPI_Barrier(MPI_world);
}
