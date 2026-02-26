#pragma once
#include <array>

#include "Ref.h"


class Box : public Ref {
public:
	Box(Engine& engine_);
	Box(Engine& engine_,
		const std::array<double, 3>& min_,
		const std::array<double, 3>& max_);
	Box(Engine& engine_, std::vector<std::string> args_);
	virtual void injectDependencies(Engine& engine_);
	void getDimensions(std::array<double, 3>& min_,
		std::array<double, 3>& max_) const;

private:
	std::array<double, 3> min, max;
};
