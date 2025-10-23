#pragma once

#include <mdspan>
#include <vector>
#include "array.h"

template<allocation_major alloc_major>
class array_mdspan : public array
{
public:
	array_mdspan(const int& _n_xs, const int& _n_ys, const std::string& _output) :
		array{ _n_xs, _n_ys, _output } {}
	array_mdspan(const array_c_y_major& _in) = delete;
	array_mdspan& operator=(const array_c_y_major& _in) = delete;
	array_mdspan(array_c_y_major&& _in) = default;
	array_mdspan& operator=(array_c_y_major&& _in) = default;
	~array_mdspan() {}
	void initiate() override {
		allocate();
		if (alloc_major == allocation_major::X_MAJOR)
			mdspn = std::mdspan(data.data(), n_xs, n_ys);
		else if (alloc_major == allocation_major::Y_MAJOR)
			mdpsn = std::mdspan(data.data(), n_ys, n_xs);
	}
	double& operator()(int x, int y) override {
		if (bounds_check_flag)
			bounds_check(x, y); // Check bounds before accessing
		if (alloc_major == allocation_major::X_MAJOR)
			return mdpsn[x][y];
		else if (alloc_major == allocation_major::Y_MAJOR)
			return mdspn[y][x];
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
	std::mdspan mdspn;
};