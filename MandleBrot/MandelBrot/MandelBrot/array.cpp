#include "array.h"

void array::write_data(const int& _xhi, const int& _yhi)
{
	if (!output || !output->is_open()) {
		throw std::runtime_error("Output stream is not open.");
	}

	for (int j = 0; j < _yhi; j++)
	{
		for (int i = 0; i < _xhi - 1; i++)
			*output << (*this)(i, j) << ",";
		*output << (*this)(_xhi - 1, j) << std::endl;
	}
}
void array::write_data()
{
	write_data(n_xs, n_ys);
}
void array::bounds_check(int x, int y) const
{
	if (x < 0 || x >= n_xs || y < 0 || y >= n_ys)
		throw std::out_of_range("Index out of range in array_c_x_major");
}
double& array::operator_c_cpp_x_major(int x, int y)
{
	bounds_check(x, y); // Check bounds before accessing
	return data[x][y];
}
double& array::operator_c_cpp_y_major(int x, int y)
{
	bounds_check(x, y); // Check bounds before accessing
	return data[y][x];
}
double& array::operator_modern_x_major(int x, int y)
{
	bounds_check(x, y); // Check bounds before accessing
	return data_modern[x * n_ys + y];
}
double& array::operator_modern_y_major(int x, int y)
{
	bounds_check(x, y); // Check bounds before accessing
	return data_modern[y* n_xs + x];
}