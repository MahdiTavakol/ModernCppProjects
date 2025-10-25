#pragma once

#include "array.h"

namespace Array_NS {

	template<allocation_major alloc_major>
	class array_modern : public array
	{
	public:
		array_modern(const int& _n_xs, const int& _n_ys, const std::string& _output) :
			array{ _n_xs, _n_ys, _output } {
		}
		array_modern(const array_modern& _in) = delete;
		array_modern& operator=(const array_modern& _in) = delete;
		array_modern(array_modern&& _in) = default;
		array_modern& operator=(array_modern&& _in) = default;
		~array_modern() {}
		double& operator()(int x, int y) override {
			if (bounds_check_flag)
				bounds_check(x, y); // Check bounds before accessing
			if (alloc_major == allocation_major::X_MAJOR)
				return data[x * n_ys + y];
			else if (alloc_major == allocation_major::Y_MAJOR)
				return data[y * n_xs + x];
		}


	protected:
		void allocate() override
		{
			data = std::make_unique<double[]>(n_xs * n_ys);
		}

		void deallocate() override {}

	private:
		bool bounds_check_flag = false;
		std::unique_ptr<double[]> data;
	};
}