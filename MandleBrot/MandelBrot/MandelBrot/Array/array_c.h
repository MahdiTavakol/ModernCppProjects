#pragma once

#include "array.h"

namespace Array_NS {

	template<allocation_major alloc_major>
	class array_c : public array
	{
	public:
		array_c(const int& _n_xs, const int& _n_ys, const std::string& _output) :
			array{ _n_xs, _n_ys, _output } {
		}
		array_c(const array_c& _in) = delete;
		array_c& operator=(const array_c& _in) = delete;
		array_c(array_c&& _in) = default;
		array_c& operator=(array_c&& _in) = default;
		~array_c() {
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
			double* temp = (double*)malloc(n_xs * n_ys * sizeof(double));
			if (alloc_major == allocation_major::X_MAJOR) {
				data = (double**)malloc(n_xs * sizeof(double*));
				for (int i = 0; i < n_xs; i++)
					data[i] = &temp[i * n_ys];
			}
			else if (alloc_major == allocation_major::Y_MAJOR) {
				data = (double**)malloc(n_ys * sizeof(double*));
				for (int i = 0; i < n_ys; i++)
					data[i] = &temp[i * n_xs];
			}
		}
		void deallocate() override {
			if (data)
			{
				free(data[0]);
				free(data);
				data = nullptr;
			}
		}

	private:
		bool bounds_check_flag = false;
		double** data;
	};
}