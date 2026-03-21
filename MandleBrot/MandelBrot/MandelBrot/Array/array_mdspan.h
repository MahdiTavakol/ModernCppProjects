#pragma once

#include <mdspan>
#include <vector>
#include "array.h"

namespace Array_NS {

	template<allocation_major alloc_major>
	class array_mdspan final : public array
	{
	public:
		array_mdspan(const int& _n_xs, const int& _n_ys, const std::string& _output) :
			array{ _n_xs, _n_ys, _output } {
		}
		array_mdspan(const array_mdspan& _in) = delete;
		array_mdspan& operator=(const array_mdspan& _in) = delete;
		array_mdspan(array_mdspan&& _in) = default;
		array_mdspan& operator=(array_mdspan&& _in) = default;
		~array_mdspan() override {}
		void initiate() override {
			allocate();
		}
		double& operator()(int x, int y) override {
			if (bounds_check_flag)
				bounds_check(x, y); // Check bounds before accessing
			if (alloc_major == allocation_major::X_MAJOR) {
				auto mdspn = std::mdspan(data.data(), n_xs, n_ys);
				return mdspn[x, y];
			}
			else if (alloc_major == allocation_major::Y_MAJOR) {
				auto mdspn = std::mdspan(data.data(), n_ys, n_xs);
				return mdspn[y, x];
			}
		}


	protected:
		void allocate() override
		{
			data.resize(n_xs * n_ys);
		}

		void deallocate() override {}

	private:
		bool bounds_check_flag = false;
		std::vector<double> data;
	};
}