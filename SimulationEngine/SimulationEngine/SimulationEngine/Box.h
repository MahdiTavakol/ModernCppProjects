#pragma once
#include <array>

#include "Ref.h"


class Box : public Ref {
public:
	Box();
	Box(const std::array<double, 3>& min_,
		const std::array<double, 3>& max_);
	Box(std::vector<std::string> args_);
	void getDimensions(std::array<double, 3>& min_,
		std::array<double, 3>& max_) const;

private:
	std::array<double, 3> min, max;
};
