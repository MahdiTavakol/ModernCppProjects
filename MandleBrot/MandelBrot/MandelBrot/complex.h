#ifndef COMPLEX_H
#define COMPLEX_H

#include <sstream>
#include <cmath>

class complex {
public:
    complex() : real(0.0), imag(0.0) {}
    complex(double _real, double _imag) : real(_real), imag(_imag) {}
    complex(double _real) : real(_real), imag(0) {}
    complex(std::string _complex_number)
    {
        double real, imag;
        char i_char;
        char i_sign;
        std::stringstream iss(_complex_number);
        if (!(iss >> real >> i_sign >> i_char >> imag) || (i_sign != '+' && i_sign != '-') || (i_char != 'i' && i_char != 'j'))
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

    double real, imag;  // Real and imaginary parts
};

#endif


