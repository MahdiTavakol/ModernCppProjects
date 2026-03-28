#include "parallel.h"


int parallel::return_rank() const {
    return rank;
}

int parallel::return_size() const {
    return size;
}


std::array<int, 2> parallel::return_rank_config() const
{
    return rank_config;
}

std::array<int, 2> parallel::return_size_config() const
{
    return size_config;
}

