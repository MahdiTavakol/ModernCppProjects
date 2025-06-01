#include "array_c_y_major.h"

array_c_y_major::array_c_y_major(const int& _n_xs, const int& _n_ys, const std::string& _output)
	: array{ _n_xs, _n_ys, _output } {
	allocate();
}

double& array_c_y_major::operator()(int x, int y)
{
	return operator_c_cpp_y_major(x, y);
}

void array_c_y_major::allocate()
{
	double* temp = (double*)malloc(n_xs * n_ys * sizeof(double));
	data = (double**)malloc(n_ys * sizeof(double*));

	for (int i = 0; i < n_ys; i++)
		data[i] = &temp[i * n_xs];
}

void array_c_y_major::deallocate()
{
	if (data)
	{
		free(data[0]);
		free(data);
		data = nullptr;
	}
}

