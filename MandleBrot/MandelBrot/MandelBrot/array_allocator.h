#pragma once
#include <memory>

#include "array.h"
#include "array_c_x_major.h"
#include "array_c_y_major.h"
#include "array_cpp_x_major.h"
#include "array_cpp_y_major.h"
#include "array_c_modern_x_major.h"
#include "array_c_modern_y_major.h"


enum class allocation_mode { C_X_MAJOR, C_Y_MAJOR, CPP_X_MAJOR, CPP_Y_MAJOR, MODERN_C_X_MAJOR, MODERN_C_Y_MAJOR };

class array_allocator
{
public:
	array_allocator(allocation_mode mode_, const int& n_xs_, const int& n_ys_, const std::string& output_name_)
		: mode{ mode_ }, n_xs{ n_xs }, n_ys{ n_ys }, output_name{ output_name_} 
	{
		switch (mode)
		{
		case allocation_mode::C_X_MAJOR:
			array_ptr = std::make_unique<array_c_x_major>(n_xs, n_ys, output_name);
			break;
		case allocation_mode::C_Y_MAJOR:
			array_ptr = std::make_unique<array_c_y_major>(n_xs, n_ys, output_name);
			break;
		case allocation_mode::CPP_X_MAJOR:
			array_ptr = std::make_unique<array_cpp_x_major>(n_xs, n_ys, output_name);
			break;
		case allocation_mode::CPP_Y_MAJOR:
			array_ptr = std::make_unique<array_cpp_y_major>(n_xs, n_ys, output_name);
			break;
		case allocation_mode::MODERN_C_X_MAJOR:
			array_ptr = std::make_unique<array_c_modern_x_major>(n_xs, n_ys, output_name);
			break;
		case allocation_mode::MODERN_C_Y_MAJOR:
			array_ptr = std::make_unique<array_c_modern_y_major>(n_xs, n_ys, output_name);
			break;
		default:
			throw std::runtime_error("Invalid allocation mode");
		}
	}
	array_allocator(const array_allocator& _in) = delete;
	array_allocator& operator=(const array_allocator& _in) = delete;
	array_allocator(array_allocator&& _in) = delete;
	array_allocator& operator=(array_allocator&& _in) = delete;
	~array_allocator() = default;

	double& operator()(int x, int y)
	{
		return (*array_ptr)(x, y);
	}

	void write_data(const int& xhi, const int& yhi)
	{
		array_ptr->write_data(xhi, yhi);
	}

private:
	allocation_mode mode;
	int n_xs, n_ys;
	std::string output_name;
	std::unique_ptr<array> array_ptr;
};