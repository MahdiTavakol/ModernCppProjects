#pragma once
#include <array>

#include "Type.h"

using std::array;

class Box : public Type {
public:
	Box(Engine& engine_,
		const array<double, 3>& min_,
		const array<double, 3>& max_) :
		Type{ engine_, "Box-1" },
		min{ min_ }, max{ max_ }
	{
	}
	void getDimensions(array<double, 3>& min_,
		array<double, 3>& max_) const {
		min_ = min;
		max_ = max;
	}
private:
	array<double, 3> min, max;
};
