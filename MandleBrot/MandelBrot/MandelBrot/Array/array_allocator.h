#pragma once
#include <memory>

#include "array.h"
#include "array_c.h"
#include "array_cpp.h"
#include "array_modern.h"
#include "../definitions.h"

#if USE_MDSPAN
	#include "array_mdspan.h"
#endif


namespace Array_NS {

	class array_allocator
	{
	public:
		array_allocator(allocation_mode mode_, const allocation_major& major_,
			const int& n_xs_, const int& n_ys_, const std::string& output_name_)
			: mode{ mode_ }, major{ major_ },
			n_xs{ n_xs }, n_ys{ n_ys },
			output_name{ output_name_ }
		{
			if (major == allocation_major::X_MAJOR)
			{
				switch (mode) {
				case allocation_mode::C:
					array_ptr = std::make_unique<array_c<allocation_major::X_MAJOR>>(n_xs, n_ys, output_name);
					break;
				case allocation_mode::CPP:
					array_ptr = std::make_unique<array_cpp<allocation_major::X_MAJOR>>(n_xs, n_ys, output_name);
					break;
				case allocation_mode::MODERN:
					array_ptr = std::make_unique<array_modern<allocation_major::X_MAJOR>>(n_xs, n_ys, output_name);
					break;
#if USE_MDSPAN
				case allocation_mode::MDSPAN:
					array_ptr = std::make_unique<array_mdspan<allocation_major::X_MAJOR>>(n_xs, n_ys, output_name);
					break;
#endif
				default:
					throw std::runtime_error("Invalid allocation mode");
				}
			}
			else if (major == allocation_major::Y_MAJOR) {
				switch (mode) {
				case allocation_mode::C:
					array_ptr = std::make_unique<array_c<allocation_major::Y_MAJOR>>(n_xs, n_ys, output_name);
					break;
				case allocation_mode::CPP:
					array_ptr = std::make_unique<array_cpp<allocation_major::Y_MAJOR>>(n_xs, n_ys, output_name);
					break;
				case allocation_mode::MODERN:
					array_ptr = std::make_unique<array_modern<allocation_major::Y_MAJOR>>(n_xs, n_ys, output_name);
					break;
				case allocation_mode::MDSPAN:
					array_ptr = std::make_unique<array_mdspan<allocation_major::Y_MAJOR>>(n_xs, n_ys, output_name);
					break;
				default:
					throw std::runtime_error("Invalid allocation mode");
				}
			}
			else
				throw std::runtime_error("Invalid allocation major");
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
		allocation_major major;
		allocation_mode mode;
		int n_xs, n_ys;
		std::string output_name;
		std::unique_ptr<array> array_ptr;
	};
}
