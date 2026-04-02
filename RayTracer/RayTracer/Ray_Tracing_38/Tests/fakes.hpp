
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