#pragma once

#include "array.h"

namespace Array_NS {

	template<allocation_major alloc_major>
	class array_cpp : public array
	{
	public:
		array_cpp(const int& _n_xs, const int& _n_ys, const std::string& _output) :
			array{ _n_xs, _n_ys, _output }
		{
		}
		array_cpp(const array_cpp& _in) = delete;
		array_cpp& operator=(const array_cpp& _in) = delete;
		array_cpp(array_cpp& _in) = default;
		array_cpp& operator=(array_cpp&& _in) = default;
		~array_cpp() {
			deallocate();
		}
		double& operator()(int x, int y) override {
			if (bounds_check_flag)
				bounds_check(x, y); // Check bounds before accessing
			if (alloc_major == allocation_major::X_MAJOR)
				return data[x][y];
			else if (alloc_major == allocation_major::Y_MAJOR)
				return data[y][x];
		}

	protected:
		void allocate() override
		{
			double* temp = new double[n_xs * n_ys];
			if (alloc_major == allocation_major::X_MAJOR) {
				data = new double* [n_xs];
				for (int i = 0; i < n_xs; i++)
					data[i] = &temp[i * n_ys];
			}
			else if (alloc_major == allocation_major::Y_MAJOR) {
				data = new double* [n_ys];
				for (int i = 0; i < n_ys; i++)
					data[i] = &temp[i * n_xs];
			}
		}
		void deallocate() override {
			if (data)
			{
				delete[] data[0];
				delete[] data;
			}
		}

	private:
		bool bounds_check_flag = false;
		double** data;
	};
}