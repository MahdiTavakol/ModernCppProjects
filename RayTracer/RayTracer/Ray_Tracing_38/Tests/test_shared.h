#pragma once

#include "catch_amalgamated.hpp"
#include "../Input/input.h"
#include "../Geometry/triangle_mesh.h"
#include "../Algorithms/parallel.h"



class fake_camera_settings : public camera_settings
{
public:
	fake_camera_settings(color background_, 
						 color rendercolor_) :
		camera_settings{}
	{}
};



class fake_camera : public camera
{
public:
	fake_camera(vec3 min_, vec3 max_,
		std::array<int, 2> imageSize_,
		color background_ = color{ 0.0,0.0,0.0 },
		color rendercolor_ = color{ 1.0,1.0,1.0 }) :
		camera{},
		min{ min_ }, max{ max_ },
		imageSize{ imageSize_ },
		background{ background_ },
		rendercolor{ rendercolor_ }
	{
		initialize_storage();
	}

	virtual void render(const hittable& object) override
	{
		double xPerDot = (max.x() - min.x()) / static_cast<double>(imageSize[0]);
		double yPerDot = (max.y() - min.y()) / static_cast<double>(imageSize[1]);
		for (int j = 0; j < imageSize[1]; j++)
		{
			for (int i = 0; i < imageSize[0]; i++)
			{
				double locX = i * xPerDot;
				double locY = j * yPerDot;
				vec3 ray_origin{ locX,locY,50.0 };
				vec3 ray_direction{ 0.0,0.0,-1.0 };
				double ray_time{ 0.0 };
				ray myray{ ray_origin,ray_direction,ray_time };
				color pixel_color = ray_color(myray, 0.0, object);
				color_data** c_data = c_array->return_array();
				c_data[i][j].r = pixel_color.x();
				c_data[i][j].g = pixel_color.y();
				c_data[i][j].b = pixel_color.z();
			}
		}
	}

	virtual color ray_color(const ray& r_, int depth_, const hittable& object_) const override
	{
		hit_record rec;
		if (!object_.hit(r_, interval(0.001, infinity), rec))
			return background;
		return rendercolor;
	}


protected:
	void initialize_storage() override
	{
		c_array = std::make_unique<color_array>(imageSize[0], imageSize[1]);
	}

private:
	vec3 min, max;
	std::array<int, 2> imageSize;
	color background;
	color rendercolor;
};

class fake_material : public material
{
public:
	virtual ~fake_material() override = default;

	virtual bool is_equal(const material& _second) const override
	{
		return false;
	}

	bool compare(material* rhs_, const double tol_) const override
	{
		fake_material* o = dynamic_cast<fake_material*>(rhs_);
		if (!o) {
			std::cout << "Material type mismatch" << std::endl;
			return true;
		}
		return false;
	}
};

class fake_parallel : public parallel
{
public:
	fake_parallel() :
		parallel{}
	{
	}
	int return_rank() const override {
		return 0;
	}
	int return_size() const override {
		return 1;
	}
	std::array<int, 2> return_rank_config() const override {
		return std::array<int, 2>{0, 0};
	}
	std::array<int, 2> return_size_config() const override {
		return std::array<int, 2>{1, 1};
	}

	void gather(
		color_data* one_,
		color_data* one_all,
		const int& num_data
	) const override
	{
		return;
	}
	void gather(color_data** one_,
		color_data** one_all_,
		const int& width_per_rank_,
		const int& height_per_rank_) const override
	{
		return;
	}
	void barrier() const override {
		return;
	}
};

class intMatMatcher : public Catch::Matchers::MatcherGenericBase
{
public:
	intMatMatcher(int** expected_, int size_x_, int size_y_) :
		expected{ expected_ }, size_x{ size_x_ }, size_y{ size_y_ } {
	}
	bool match(int** value_) const
	{
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				if (value_[i][j] != expected[i][j])
					return false;
		return true;
	}
	std::string describe() const override
	{
		std::string output = "A custom 2D array matcher";
		return output;
	}
private:
	int size_x, size_y;
	int** expected;
};

class vec3Matcher : public Catch::Matchers::MatcherGenericBase {
public:
	vec3Matcher(const vec3& expected_, double tol_) :
		tol{ tol_ },
		expectedRef{ std::ref(expected_) }
	{
	}

	bool match(const vec3& value_) const
	{
		return equal(expectedRef.get(), value_, tol);
	}

	std::string describe() const override
	{
		std::string message = "Comparing two vec3 with the tolerance of " + std::to_string(tol);
		return message;
	}

private:
	double tol;
	std::reference_wrapper<const vec3> expectedRef;
};

class colorArrayMatcher : public Catch::Matchers::MatcherGenericBase {
public:
	colorArrayMatcher(
		color_array* expected_,
		double tol_,
		std::vector<std::array<int, 2>>& ignoredCoors_
	) :
		tol{ tol_ },
		expected{ expected_ },
		ignoredCoors{ std::move(ignoredCoors_) }
	{
	}

	colorArrayMatcher(
		color_array* expected_,
		double tol_
	) :
		tol{ tol_ },
		expected{ expected_ }
	{
		ignoredCoors = std::vector<std::array<int, 2>>{};
	}

	bool match(color_array* value_) const
	{
		return expected->equal(value_, tol, ignoredCoors);
	}

	std::string describe() const override
	{
		std::string message = "Comparing two color_arrays with the tolerance of " + std::to_string(tol);
		return message;
	}

private:
	double tol;
	color_array* expected;
	std::vector<std::array<int, 2>> ignoredCoors;
};

class hittableListMatcher : public Catch::Matchers::MatcherGenericBase {
public:
	hittableListMatcher(hittable_list* expected_, double tol_):
		expected{expected_}, tol{tol_}
	{
		if (expected_ == nullptr)
			throw std::runtime_error("The expected in the hittableListMatcher is nullptr!");
	}
	
	bool match(hittable_list* value_) const
	{
		if (value_ == nullptr)
			throw std::runtime_error("The value in the hittableListMatcher is nullptr!");

		int size = value_->size();
		int expectedsize = expected->size();

		if (size != expectedsize) {
			std::cout << "Hittable_list size mismatch" << std::endl;
			return false;
		}

		for (int i = 0; i < size; i++)
		{
			hittable* objectVal = (*value_)[i];
			hittable* objectExpected = (*expected)[i];

			material* matVal      = objectVal->get_material();
			material* matExpected = objectExpected->get_material();

			if (objectVal->compare(objectExpected, tol)) {
				std::cout << "Object mismatch" << std::endl;
				return false;
			}

			if (matVal == nullptr)
				std::cout << "The original material pointer is nullptr" << std::endl;
			if (matExpected == nullptr)
				std::cout << "The expected material pointer is nullptr" << std::endl;

			if (matVal->compare(matExpected, tol)) {
				std::cout << "Material mismatch" << std::endl;
				return false;
			}

		}
		return true;
	}

	std::string describe() const override
	{
		std::string message = "Comparing two hittable_list with tolerance of " + std::to_string(tol);
		return message;
	}

protected:
	hittable_list* expected;
	double tol;
};


class point3Matcher : public Catch::Matchers::MatcherGenericBase {
public:
	point3Matcher(const point3& expected_, double tol_) :
		tol{ tol_ },
		expectedRef{ std::ref(expected_) }
	{
	}
	bool match(const point3& value_) const
	{
		if (std::abs(expectedRef.get().x() - value_.x() >= tol))
			return false;
		else if (std::abs(expectedRef.get().y() - value_.y() >= tol))
			return false;
		else if (std::abs(expectedRef.get().z() - value_.z() >= tol))
			return false;
		return true;
	}
	std::string describe() const override
	{
		std::string message = "Comparing two point3 with the tolerance of " + std::to_string(tol);
		return message;
	}

private:
	std::reference_wrapper<const point3> expectedRef;
	double tol;
};