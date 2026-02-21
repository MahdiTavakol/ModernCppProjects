#include "Box.h"
#include "Error.h"
#include "Engine.h"

Box::Box(Engine& engine_) :
	Ref{engine_,"1"}
{}

Box::Box(Engine& engine_,
	const std::array<double, 3>& min_,
	const std::array<double, 3>& max_) :
	Ref{ engine_, "Box-1" },
	min{ min_ }, max{ max_ }
{}

Box::Box(Engine& engine_,
	std::vector<std::string> args_):
	Ref{ engine_, "Box-1" }
{
	auto& error = engine().getError();
	int narg = args_.size();
	if (narg < 7)
		error->one("Not enough arguments for box command! Expected 7, got " + std::to_string(narg));
	min = std::array<double, 3>{ std::stod(args_[1]), std::stod(args_[2]), std::stod(args_[3]) };
	max = std::array<double, 3>{ std::stod(args_[4]), std::stod(args_[5]), std::stod(args_[6]) };
}


void Box::getDimensions(std::array<double, 3>& min_,
	std::array<double, 3>& max_) const {
	min_ = min;
	max_ = max;
}