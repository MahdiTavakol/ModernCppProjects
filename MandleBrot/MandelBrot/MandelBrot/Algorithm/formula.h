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
		virtual complex<T> operator()(const complex<T>& z, const complex<T>& c) = 0;
		virtual ~formula() = default;
	};


	// mandelbrot formula
	template<typename T = double>
		requires (std::is_arithmetic_v<T>)
	class formula_1 : public formula<T> {
	public:
		formula_1() = default;
		complex<T> operator()(complex<T>& z, const complex<T>& c) override
		{
			return z * z + c;
		}
		virtual ~formula_1() = default;
	};

	// burning ship formula
	template<typename T = double>
		requires (std::is_arithmetic_v<T>)
	class formula_2 : public formula<T> {
	public:
		formula_2() = default;
		complex<T> operator()(const complex<T>& z, const complex<T>& c) override {
			return z.abs() * z.abs() + c;
		}
		virtual ~formula_2() = default;
	};
}