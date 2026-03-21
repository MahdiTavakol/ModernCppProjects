#include "array.h"
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace Array_NS;

constexpr double etol = 1e-6;

void array::write_data(const int& _xhi, const int& _yhi, const double& gamma_)
{
	if (!output || !output->is_open()) {
		throw std::runtime_error("Output stream is not open.");
	}

	double val;
	for (int j = 0; j < _yhi; j++)
	{
		for (int i = 0; i < _xhi - 1; i++) {
			if (std::abs(gamma_ - 1.0) < etol)
				val = (*this)(i, j);
			else 
				val = std::pow((*this)(i, j), 1.0 / gamma_);
			*output << std::fixed << std::setprecision(24) << val << ",";
		}
		*output << (*this)(_xhi - 1, j) << std::endl;
	}
}

void array::write_data(const double& gamma_)
{
	write_data(n_xs, n_ys,gamma_);
}

void array::bounds_check(int x, int y) const
{
	if (x < 0 || x >= n_xs || y < 0 || y >= n_ys) {
		std::stringstream ss;
		ss << "Index out of range in " << typeid(*this).name() << std::endl;
		throw std::out_of_range(ss.str().c_str());
	}
}
