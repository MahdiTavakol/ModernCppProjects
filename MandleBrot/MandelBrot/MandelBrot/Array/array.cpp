#include "array.h"
#include <sstream>

using namespace Array_NS;

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
	if (x < 0 || x >= n_xs || y < 0 || y >= n_ys) {
		std::stringstream ss;
		ss << "Index out of range in " << typeid(*this).name() << std::endl;
		throw std::out_of_range(ss.str().c_str());
	}
}
