#pragma once

#include "../Numerical/complex.h"
#include <type_traits>


using namespace Numerical_NS;

namespace Mandelbrot_NS {

	template<typename T = double>
		requires (std::is_arithmetic_v<T>)
	class formula {
	public:
		formula() = default;
		virtual complex<T> operator(const complex<T>& z, const complex<T>& c) = 0;
	};

	template<typename T = double>
		requires (std::is_arithmetic_v<T>)
	class formula_1 : public formula<T> {
		formula_1() = default;
		complex<T> operator(complex<T>& z, const complex<T>& c) override
		{
			return z * z + c;
		}
	};

	template<typename T = double>
		requires (std::is_arithmetic_v<T>)
	class formula_2 : public formula<T> {
		formula_2() = default;
		complex<T> operator(const complex<T>& z, const complex<T>& c) override {
			return z.abs() * z.abs() + c;
		}
	};
}