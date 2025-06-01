#pragma once

#include "array.h"

class array_c_modern_x_major : public array
{
public:
	array_c_modern_x_major(const int& _n_xs, const int& _n_ys, const std::string& _output);
	array_c_modern_x_major(const array_c_modern_x_major& _in) = delete;
	array_c_modern_x_major& operator=(const array_c_modern_x_major& _in) = delete;
	array_c_modern_x_major(array_c_modern_x_major&& _in) = default;
	array_c_modern_x_major& operator=(array_c_modern_x_major&& _in) = default;
	~array_c_modern_x_major() = default;
	double& operator()(int x, int y) override;

protected:
	void allocate() override;
	void deallocate() override;
};