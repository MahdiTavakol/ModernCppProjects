#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include "definitions.h"

enum { C_X_MAJOR, C_Y_MAJOR, CPP_X_MAJOR, CPP_Y_MAJOR };


class array
{
public:
	array(const int& _allocation_mode, const int& _n_xs, const int& _n_ys, std::ostream* _output);
	array();
	array(const array& _in);
	~array();
	void write_data(const int& _xhi, const int& _yhi);
	void write_data();
	double& operator()(int x, int y);
	//double operator()(int& x, int& y) const;
	double** data;

protected:
	int allocation_mode;
	int n_xs, n_ys;
	std::ostream* output;
	void allocate_v1();
	void allocate_v2();
	void allocate_v3();
	void allocate_v4();
	void allocate_v5();
	void allocate_v6();
	void deallocate_v1();
	void deallocate_v2();
	void deallocate_v3();
	void deallocate_v4();
	void deallocate_v5();
	void deallocate_v6();
};


#endif
