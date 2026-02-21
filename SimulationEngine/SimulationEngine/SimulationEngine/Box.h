#pragma once
#include <array>

#include "Ref.h"


class Box : public Ref {
public:
	Box(Engine& engine_,
		const std::array<double, 3>& min_,
		const std::array<double, 3>& max_) :
		Ref{ engine_, "Box-1" },
		min{ min_ }, max{ max_ }
	{
	}
	void getDimensions(std::array<double, 3>& min_,
		std::array<double, 3>& max_) const {
		min_ = min;
		max_ = max;
	}
private:
	std::array<double, 3> min, max;
};
