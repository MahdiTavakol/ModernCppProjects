#pragma once

#include "catch_amalgamated.hpp"

#include "..\Algorithms\communicator.h"

void minRanksRequirement(std::unique_ptr<communicator>& para_, const std::array<int, 2>& minSize_);

std::unique_ptr<communicator> skipExtraRanks(std::unique_ptr<communicator>& para_, const std::array<int,2>& maxSize_);

void printRankZero(std::unique_ptr<communicator>& para_, const std::string text);

// allocater and deallocator lambda functions
void allocate(color_data**& data_, int width_, int height_);

void deallocate(color_data**& data_);