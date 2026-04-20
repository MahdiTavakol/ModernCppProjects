
#include "catch_amalgamated.hpp"
#include "../Input/input.h"
#include "../Geometry/triangle_mesh.h"
#include "../Algorithms/communicator.h"
#include <exception>

class fake_camera_settings : public camera_settings
{
public:
	fake_camera_settings(color background_,
		color rendercolor_) :
		camera_settings{}
	{
	}
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
	{}

	virtual void render(const hittable& object) override
	{
		double xPerDot = (max.x() - min.x()) / static_cast<double>(imageSize[0]);
		double yPerDot = (max.y() - min.y()) / static_cast<double>(imageSize[1]);
		color_data** c_data = img->returnColorData();
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
				color pixel_color = ray_color(myray, 0, object);
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

class fake_parallel : public communicator
{
public:
	fake_parallel() :
		communicator{}
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

	std::unique_ptr<communicator> split(const std::array<int, 2>& maxRanks_) const override
	{
		return std::make_unique<fake_parallel>();
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

class fake_color_array : public color_array
{
public:
	// attention.. The colors is organized in a row major fashion.
	fake_color_array(const int& width_, const int& height_, std::vector<double>& colors_):
		color_array{width_,height_}
	{
		int nData = static_cast<int>(colors_.size());
		if (width_ * height_ * 3 != nData)
			throw std::invalid_argument("Corrupted data!");

		for (int i = 0; i < width_; i++) {
			for (int j = 0; j < height_; j++) {
				int loc = i + j * width_;
				double r = colors_[3 * loc];
				double g = colors_[3 * loc + 1];
				double b = colors_[3 * loc + 2];
				array[i][j] = color_data{ r,g,b };
			}
		}
	}

private:

};