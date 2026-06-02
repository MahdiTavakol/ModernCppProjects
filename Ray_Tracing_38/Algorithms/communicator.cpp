#include "communicator.h"


communicator::communicator(std::array<int, 2> size_config_):
     size_config{size_config_}
{}

communicator::communicator(settings* comm_settings_)
{
    // checking the setting type
    communicator_settings* sett = dynamic_cast<communicator_settings*>(comm_settings_);
    if (!sett)
        throw std::invalid_argument("Wrong settings object");


    sett->return_size_config(size_config);
}

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

void communicator::reset_size_ratio(std::array<int, 2> new_size_ratio_)
{
}


std::array<int, 2> communicator::return_rank_config() const
{
    return rank_config;
}

std::array<int, 2> communicator::return_size_config() const
{
    return size_config;
}

