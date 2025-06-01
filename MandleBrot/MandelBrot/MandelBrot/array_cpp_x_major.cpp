#include "array_cpp_x_major.h"

array_cpp_x_major::array_cpp_x_major(const int& _n_xs, const int& _n_ys, const std::string& _output)
	: array{ _n_xs, _n_ys, _output } {
	allocate();
}

double& array_cpp_x_major::operator()(int x, int y)
{
	return operator_c_cpp_x_major(x, y);
}

void array_cpp_x_major::allocate()
{
	double* temp = new double[n_xs * n_ys];
	data = new double* [n_xs];
	for (int i = 0; i < n_xs; i++)
		data[i] = &temp[i * n_ys];
}

void array_cpp_x_major::deallocate()
{
	if (data)
	{
		delete [] data[0];
		delete [] data;
		data = nullptr;
	}
}

