#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include "../definitions.h"


namespace Array_NS {

	enum class allocation_mode
	{
		C,
		CPP,
		MODERN,
		MDSPAN,
	};

	enum class allocation_major {
		X_MAJOR,
		Y_MAJOR
	};

	class array
	{
	public:
		array(const int& _n_xs, const int& _n_ys, const std::string& output_name_) :
			n_xs{ _n_xs }, n_ys{ _n_ys },
			output{ std::make_unique<std::ofstream>(output_name_) } {}
		array(const array& _in) = delete;
		array& operator=(const array& _in) = delete;
		virtual ~array() {}
		virtual void initiate() { allocate(); }
		void write_data(const int& _xhi, const int& _yhi, const double& gamma_ = 1.0);
		void write_data(const double& gamma_ = 1.0);
		virtual double& operator()(int x, int y) = 0;
		//double operator()(int& x, int& y) const;
		void reset(const std::string& file_name) {
			output = std::make_unique<std::ofstream>(file_name);
		}


	protected:
		int n_xs, n_ys;
		std::unique_ptr<std::ofstream> output;
		virtual void allocate() = 0;
		virtual void deallocate() = 0;
		void bounds_check(int x, int y) const;

	};
}

