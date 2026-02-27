#include "Box.h"
#include "Error.h"
#include "Engine.h"
#include <stdexcept>

Box::Box() :
	Ref{"Box-1"}
{}

Box::Box(const std::array<double, 3>& min_,
	     const std::array<double, 3>& max_) :
	Ref{ "Box-1" },
	min{ min_ }, max{ max_ }
{}

Box::Box(std::vector<std::string> args_):
	Ref{ "Box-1" }
{
	auto narg = args_.size();
	if (narg < 7)
		throw std::invalid_argument("Not enough arguments for box command! Expected 7, got " + std::to_string(narg));
	min = std::array<double, 3>{ std::stod(args_[1]), std::stod(args_[2]), std::stod(args_[3]) };
	max = std::array<double, 3>{ std::stod(args_[4]), std::stod(args_[5]), std::stod(args_[6]) };
}


void Box::getDimensions(std::array<double, 3>& min_,
	std::array<double, 3>& max_) const {
	min_ = min;
	max_ = max;
}