#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include "definitions.h"



class array
{
public:
	array(const int& _n_xs, const int& _n_ys, const std::string& output_name_) 
		: data{ nullptr }, output {std::make_unique<std::ofstream>(output_name_)} {};
	array(const array& _in) = delete;
	array& operator=(const array& _in) = delete;
	~array() {
		deallocate();
	}
	void write_data(const int& _xhi, const int& _yhi);
	void write_data();
	virtual double& operator()(int x, int y) = 0;
	//double operator()(int& x, int& y) const;
	std::unique_ptr<double []> data_modern;
	double** data;

protected:
	int allocation_mode;
	int n_xs, n_ys;
	std::unique_ptr<std::ofstream> output;
	virtual void allocate() = 0;
	virtual void deallocate() = 0;
	void bounds_check(int x, int y) const;
	double& operator_c_cpp_x_major(int x, int y);
	double& operator_c_cpp_y_major(int x, int y);
	double& operator_modern_x_major(int x, int y);
	double& operator_modern_y_major(int x, int y);
};

