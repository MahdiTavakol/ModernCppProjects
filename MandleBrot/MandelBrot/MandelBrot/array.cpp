#include "array.h"


array::array(const int& _allocation_mode, const int& _n_xs, const int& _n_ys, std::ostream* _output)
		: allocation_mode(_allocation_mode), n_xs(_n_xs), n_ys(_n_ys), output(_output)
	{
		data = nullptr;

		switch (allocation_mode)
		{
		case C_X_MAJOR:
			allocate_v1();
			return;
		case C_Y_MAJOR:
			allocate_v2();
			return;
		case CPP_X_MAJOR:
			allocate_v3();
			return;
		case CPP_Y_MAJOR:
			allocate_v4();
			return;
		}
	}
array::array() : array(C_X_MAJOR, 0, 0, nullptr) {}
array::array(const array& _in) : array(_in.allocation_mode, _in.n_xs, _in.n_ys, _in.output) {}
array::~array()
	{
		switch (allocation_mode)
		{
		case C_X_MAJOR:
			deallocate_v1();
			break;
		case C_Y_MAJOR:
			deallocate_v2();
			break;
		case CPP_X_MAJOR:
			deallocate_v3();
			break;
		case CPP_Y_MAJOR:
			deallocate_v4();
			break;
		}
	}
void array::write_data(const int& _xhi, const int& _yhi)
{
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
double& array::operator()(int x, int y)
	{
		switch (allocation_mode)
		{
		case C_X_MAJOR:
			return data[x][y];
		case CPP_X_MAJOR:
			return data[x][y];
		case C_Y_MAJOR:
			return data[y][x];
		case CPP_Y_MAJOR:
			return data[y][x];
		}
		return data[x][y];
	}

/*double array::operator()(int& x, int& y) const
	{
		switch (allocation_mode)
		{
		case C_X_MAJOR:
			return data[x][y];
		case CPP_X_MAJOR:
			return data[x][y];
		case C_Y_MAJOR:
			return data[y][x];
		case CPP_Y_MAJOR:
			return data[y][x];
		}
	}
	*/
void array::allocate_v1()
	{
		double* temp = (double*)malloc(n_xs * n_ys * sizeof(double));
		data = (double**)malloc(n_xs * sizeof(double*));

		for (int i = 0; i < n_xs; i++)
			data[i] = &temp[i * n_ys];
	}
void array::allocate_v2()
	{
		double* temp = (double*)malloc(n_xs * n_ys * sizeof(double));
		data = (double**)malloc(n_ys * sizeof(double*));

		for (int i = 0; i < n_ys; i++)
			data[i] = &temp[i * n_xs];
	}
void array::allocate_v3()
	{
		double* temp = new double[n_xs * n_ys];
		data = new double* [n_xs];

		for (int i = 0; i < n_xs; i++)
			data[i] = &temp[i * n_ys];
	}
void array::allocate_v4()
	{
		double* temp = new double[n_xs * n_ys];
		data = new double* [n_ys];

		for (int i = 0; i < n_ys; i++)
			data[i] = &temp[i * n_xs];
	}

void array::deallocate_v1()
	{
		free(data[0]);
		free(data);
	}
void array::deallocate_v2()
	{
		deallocate_v1();
	}
void array::deallocate_v3()
	{
		delete[] data[0];
		delete[] data;
	}
void array::deallocate_v4()
	{
		deallocate_v3();
	}
