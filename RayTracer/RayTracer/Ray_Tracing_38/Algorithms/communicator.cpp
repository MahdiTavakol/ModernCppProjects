#include "communicator.h"


communicator::communicator(std::array<int, 2> size_config_):
     size_config{size_config_}
{}

communicator::communicator(std::array<int, 2> size_config_, 
    std::array<int, 2> rank_config_):
    size_config{size_config_},
    rank_config{rank_config_}
{ }

int communicator::return_rank() const {
    return rank;
}

int communicator::return_size() const {
    return size;
}


std::array<int, 2> communicator::return_rank_config() const
{
    return rank_config;
}

std::array<int, 2> communicator::return_size_config() const
{
    return size_config;
}

