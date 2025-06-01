#include "array_c_modern_y_major.h"

array_c_modern_y_major::array_c_modern_y_major(const int& _n_xs, const int& _n_ys, const std::string& _output)
	: array{ _n_xs, _n_ys, _output } {
	allocate();
}

double& array_c_modern_y_major::operator()(int x, int y)
{
	return operator_modern_y_major(x, y);
}

void array_c_modern_y_major::allocate()
{
	data_modern = std::make_unique<double[]>(n_xs * n_ys);
}

void array_c_modern_y_major::deallocate()
{
	// No explicit deallocation needed for std::unique_ptr
}

