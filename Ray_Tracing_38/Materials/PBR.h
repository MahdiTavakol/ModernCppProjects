#pragma once
#include "material.h"
#include <functional>

// this converts texture to vector4
using TexVec_func = std::function<vec4(double u_, double v_, double u1_, double v1_)>;
using Sampler_func = std::function<void(int& x_)>;

class PBR_Resources
{
public:
	PBR_Resources(Logger* error_);
	~PBR_Resources();
	void set_textures(const tg3_texture* textures_, const int& texture_count_);
	void set_samplers(const tg3_sampler* samplers_, const int& sampler_count_);
	void set_images(Logger* error_, const tg3_model* model_);
	void release_images();
	tg3_texture* texture_i(size_t i_) { return &textures[i_]; }
	tg3_image_result* image_i(size_t i_) { return &images[i_]; }
	tg3_sampler* sampler_i(size_t i_) { return &samplers[i_]; }

//private:
	Logger* error;
	std::vector<tg3_texture> textures;
	std::vector<tg3_image_result> images;
	std::vector<tg3_sampler> samplers;	
};

// PBR material for gltf 
class PBR : public material
{
public:
	PBR(Logger* error_, std::shared_ptr<PBR_Resources> resources_, const tg3_material prop_);
	void init_functionals();
	TexVec_func set_TexVecFunc(const tg3_texture_info* texture_);
	void scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_) const override;
	color emitted(double _u, double _v, const point3& _p)  const override;
	bool is_equal(const material& _second) const override;


	// the main 
	static vec4 tg3_image_to_color(
		const tg3_image_result* image_,
		const tg3_sampler* smp_,
		const int& x_,
		const int& y_);

private:
	tg3_material prop;
	// shared variables
	std::shared_ptr<PBR_Resources> resources;

	//function pointers to be used in the hotspot of the rendering loop
	// to avoid checking at each step if there is a specific texture or not
	std::function<color(double, double, const point3&)> emission_func;
	std::function<vec4(const double&, const double&, const double&, const double&)> albedo_func;
	std::function<vec2(const double&, const double&, const double&, const double&)> metal_rough_func;
	// null function returning zero
	static color null_color(double u_, double v_, const point3& p) { return color{ 0,0,0 }; }
	static vec2 null_vec2(const double&, const double&, const double&, const double&) { return vec2{ 0,0 }; }
	static color fixed_color(double u_, double v_, const point3& p,const color col_) 
	{ return col_; }

	// texture to vec4 functions
	static vec4 null_vec4(double u_, double v_, double u1_, double v1_) { return vec4{ 1.0,1.0,1.0,1.0 }; }
	static vec4 tg3_image_to_color(double s_, double t_,
		const tg3_image_result* image_, const std::array<Sampler_func,2> smp_);

	// sampler functions
	static void null_sampler(int& i_) { return; }
	static void mirror_sampler(int& i_, const int size_);
	static void mirror_repeat_sampler(int& i_, const int size_);
	static void clamped_sampler(int& i_, const int size_);

};
