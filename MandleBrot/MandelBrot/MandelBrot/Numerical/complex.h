#pragma once 

#include <sstream>
#include <cmath>
#include <type_traits>
#include <string>
#include <stdexcept>

namespace Numerical_NS {

    template<typename T = double>
        requires (std::is_arithmetic_v<T>)
    class complex {
    public:
        complex() : real(T(0)), imag(T(0)) {}
        complex(T _real, T _imag) : real(_real), imag(_imag) {}
        complex(T _real) : real(_real), imag(T(0)) {}
        complex(std::string _complex_number)
        {
            char i_char;
            char i_sign;
            std::stringstream iss(_complex_number);
            if (!(iss >> real >> i_sign >> imag >> i_char) || (i_sign != '+' && i_sign != '-') || (i_char != 'i' && i_char != 'j'))
                throw std::invalid_argument("Invalid complex number format");
        }

        complex operator+(const complex& arg) const {
            return complex(this->real + arg.real, this->imag + arg.imag);
        }
        complex operator*(const complex& arg) const {
            return complex(this->real * arg.real - this->imag * arg.imag,
                           this->real * arg.imag + this->imag * arg.real);
        }
        complex& operator=(const complex& arg) {
            if (this != &arg) {
                this->real = arg.real;
                this->imag = arg.imag;
            }
            return *this;
        }


        double abs_complex() const {
            return sqrt(this->real * this->real + this->imag * this->imag);
        }

        complex abs() const {
            return complex<T>(std::abs(this->real), std::abs(this->imag));
        }

        T real, imag;  // Real and imaginary parts
    };
}



