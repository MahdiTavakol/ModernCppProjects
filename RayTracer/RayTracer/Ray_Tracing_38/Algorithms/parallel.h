#ifndef PARALLEL_H
#define PARALLEL_H

#include "../Shared/rtweekend.h"


#include <iostream>
#include <mpi.h>
#include "camera.h"
#include "hittable_list.h"
#include "hittable_list_parallel.h"
#include "../Data/color_array.h"


class parallel
{
public:
    parallel();
    virtual ~parallel();
    virtual void initialize(std::unique_ptr<camera>& cam_, std::unique_ptr<hittable_list>& world_);
    virtual void gather(std::unique_ptr<camera>& cam)
    {}
    virtual color_array* return_color_array_ptr();
    int return_rank() const;
    int return_size() const;
    void barrier() const;

protected:
    int size, rank;
    MPI_Comm MPI_world;

    color_array* c_array;
};

#endif
