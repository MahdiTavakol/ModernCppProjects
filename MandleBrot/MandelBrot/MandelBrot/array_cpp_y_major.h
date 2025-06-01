#pragma once

#include "array.h"

class array_cpp_y_major : public array
{
public:
	array_cpp_y_major(const int& _n_xs, const int& _n_ys, const std::string& _output);
	array_cpp_y_major(const array_cpp_y_major& _in) = delete;
	array_cpp_y_major& operator=(const array_cpp_y_major& _in) = delete;
	array_cpp_y_major(array_cpp_y_major&& _in) = default;
	array_cpp_y_major& operator=(array_cpp_y_major&& _in) = default;
	~array_cpp_y_major() = default;
	double& operator()(int x, int y) override;

protected:
	void allocate() override;
	void deallocate() override;
};