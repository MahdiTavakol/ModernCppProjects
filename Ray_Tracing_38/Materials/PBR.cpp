#include "PBR.h"

#include "../external/stb_image.h"

constexpr double eps = 1e-6;

PBR_Resources::PBR_Resources(Logger* error_):
	error{error_}
{}

PBR_Resources::~PBR_Resources()
{
	release_images();
}

void PBR_Resources::set_textures(const tg3_texture* textures_, const int& texture_count_)
{
	for (int i = 0; i < texture_count_; i++)
	{
		textures.push_back(textures_[i]);
	}
}

void PBR_Resources::set_samplers(const tg3_sampler* samplers_, const int& sampler_count_)
{
	for (int i = 0; i < sampler_count_; i++)
	{
		samplers.push_back(samplers_[i]);
	}
}

void PBR_Resources::set_images(Logger* error_, const tg3_model* model_)
{
	int msg_level;
	std::string message;

	const tg3_image* input_images = model_->images;
	int32_t images_count = model_->images_count;


	images.resize(images_count);

	for (int i = 0; i < images_count; i++)
	{
		msg_level = 2;
		message = "Loading image-" + std::to_string(i);
		error_->print_message(message, msg_level);

		const tg3_image* image_i = &input_images[i];
		tg3_image_result& loaded_image_i = images[i];
		if (image_i->buffer_view && image_i->uri.data)
		{
			throw std::invalid_argument("Both the buffer_view and uri cannot be specified for an image at the same time!");
		}
		if (image_i->uri.data)
		{
			throw std::invalid_argument("The uri option for image is not supported yet!");
		}
		else if (image_i->buffer_view)
		{
			const tg3_buffer_view& buffer_view_i = model_->buffer_views[image_i->buffer_view];
			const tg3_buffer& buffer_i = model_->buffers[buffer_view_i.buffer];
			const auto dataAddress = buffer_i.data.data + buffer_view_i.byte_offset;
			int size = static_cast<int>(buffer_view_i.byte_length);

			int w = 0, h = 0, comp = 0, req_comp = 0;

			// decoding the image header
			if (!stbi_info_from_memory(dataAddress, size, &w, &h, &comp))
			{
				throw std::invalid_argument("Unknown format!");
			}

			int bits = 8;

			if (stbi_is_16_bit_from_memory(dataAddress, size))
			{
				bits = 16;
			}


			unsigned char* data = nullptr;
			if (bits == 16)
			{
				data = reinterpret_cast<unsigned char*>(
					stbi_load_16_from_memory(dataAddress, size, &w, &h, &comp, req_comp));
			}
			// load as 8 bit per channel
			if (!data)
			{
				data = stbi_load_from_memory(dataAddress, size, &w, &h, &comp, req_comp);
				if (!data)
				{
					throw std::invalid_argument("The stb cannot convert the image!");
				}
				bits = 8;
			}

			if ((w < 1) || (h < 1))
			{
				throw std::invalid_argument("Wrong image format");
			}

			loaded_image_i.width = w;
			loaded_image_i.height = h;
			loaded_image_i.component = comp;
			loaded_image_i.bits = bits;

			// allocating data
			int num_elements = w * h * comp * static_cast<int>(bits / 8);
			loaded_image_i.pixels = new uint8_t[num_elements];
			std::copy(data, data + num_elements, loaded_image_i.pixels);

			//cleaning up the data
			stbi_image_free(data);
		}
	}
}



void PBR_Resources::set_image(Logger* error_, const tg3_model* model_, const int image_id_)
{
	const tg3_image* input_images = model_->images;
	const tg3_image* image_i = &input_images[image_id_];
	tg3_image_result& loaded_image_i = images[image_id_];
	if (image_i->buffer_view && image_i->uri.data)
	{
		throw std::invalid_argument("Both the buffer_view and uri cannot be specified for an image at the same time!");
	}
	if (image_i->uri.data)
	{
		throw std::invalid_argument("The uri option for image is not supported yet!");
	}
	else if (image_i->buffer_view)
	{
		const tg3_buffer_view& buffer_view_i = model_->buffer_views[image_i->buffer_view];
		const tg3_buffer& buffer_i = model_->buffers[buffer_view_i.buffer];
		const auto dataAddress = buffer_i.data.data + buffer_view_i.byte_offset;
		int size = static_cast<int>(buffer_view_i.byte_length);

		int w = 0, h = 0, comp = 0, req_comp = 0;

		// decoding the image header
		if (!stbi_info_from_memory(dataAddress, size, &w, &h, &comp))
		{
			throw std::invalid_argument("Unknown format!");
		}

		int bits = 8;

		if (stbi_is_16_bit_from_memory(dataAddress, size))
		{
			bits = 16;
		}


		unsigned char* data = nullptr;
		if (bits == 16)
		{
			data = reinterpret_cast<unsigned char*>(
				stbi_load_16_from_memory(dataAddress, size, &w, &h, &comp, req_comp));
		}
		// load as 8 bit per channel
		if (!data)
		{
			data = stbi_load_from_memory(dataAddress, size, &w, &h, &comp, req_comp);
			if (!data)
			{
				throw std::invalid_argument("The stb cannot convert the image!");
			}
			bits = 8;
		}

		if ((w < 1) || (h < 1))
		{
			throw std::invalid_argument("Wrong image format");
		}

		loaded_image_i.width = w;
		loaded_image_i.height = h;
		loaded_image_i.component = comp;
		loaded_image_i.bits = bits;

		// allocating data
		int num_elements = w * h * comp * static_cast<int>(bits / 8);
		loaded_image_i.pixels = new uint8_t[num_elements];
		std::copy(data, data + num_elements, loaded_image_i.pixels);

		//cleaning up the data
		stbi_image_free(data);
	}
}

void PBR_Resources::release_images()
{
	for (tg3_image_result& image_i : images)
	{
		if (image_i.pixels != nullptr) {
			delete[] image_i.pixels;
			image_i.pixels = nullptr;
		}
	}
}

PBR::PBR(Logger* error_, std::shared_ptr<PBR_Resources> resources_, const tg3_material prop_) :
	material{ error_ },
	resources{resources_},
	prop{ prop_ }
{
	// deep copies is not possible since member functions are const pointer!

	// returning warnings for ignored extras
	tg3_extras_ext mat_ext = prop.ext;
	if (mat_ext.extras != NULL)
		std::cout << "warning: ignoring the extras for the material" << std::endl;
	tg3_extras_ext pbr_ext = prop.pbr_metallic_roughness.ext;
	if (pbr_ext.extras != NULL)
		std::cout << "Warning: ignoring extras for the pbr_metallic_roughness" << std::endl;
	tg3_extras_ext base_color_ext = prop.pbr_metallic_roughness.base_color_texture.ext;
	if (base_color_ext.extras != NULL)
		std::cout << "Warning: ignoring extras for the pbr_metallic_roughness base_color_texture" << std::endl;
	tg3_extras_ext met_rough_ext = prop.pbr_metallic_roughness.metallic_roughness_texture.ext;
	if (met_rough_ext.extras != NULL)
		std::cout << "Warning: ignoring extras for the pbr_metallic_roughness metall_roughness" << std::endl;
	tg3_extras_ext norm_ext = prop.normal_texture.ext;
	if (norm_ext.extras != NULL)
		std::cout << "Warning: ignoring the extras for the normal_texture" << std::endl;
	tg3_extras_ext occl_ext = prop.occlusion_texture.ext;
	if (occl_ext.extras != NULL)
		std::cout << "Warning: ignoring the extras for the occlusion_texture" << std::endl;
	tg3_extras_ext emmi_ext = prop.emissive_texture.ext;
	if (emmi_ext.extras != NULL)
		std::cout << "Warning: ignoring the extras for the emission_texture" << std::endl;


	// initializing functionals
	init_functionals();

}

void PBR::init_functionals()
{
	int msg_level = 1;
	std::string message;
	if (
		   (resources->images.size() == 0 &&
			resources->textures.size() == 0 && 
			resources->samplers.size() == 0) ||
		    resources == nullptr
		)
	{
		message = "It seems that the shared variables in the PBR class has not been initiated yet!\n";
		message += "They should be initiated before any PBR material is created!";
		error->print_message(message, msg_level);
	}
	
	// setting the emissive_func
	{
		const double* emissive_factor_src = prop.emissive_factor;
		double emissive_factor[3];
		std::copy_n(emissive_factor_src, 3, emissive_factor);
		const tg3_texture_info* texture = &prop.emissive_texture;
		double emissive_size = std::sqrt(
			emissive_factor[0] * emissive_factor[0] +
			emissive_factor[1] * emissive_factor[1] +
			emissive_factor[2] * emissive_factor[2]
		);
		// it is not an emissive object
		if (emissive_size < eps)
		{
			emission_func = PBR::null_color;
		}
		// the emissive texture is absent
		else if (prop.emissive_texture.index == -1)
		{
			color emissive_color = color(emissive_factor[0], emissive_factor[1], emissive_factor[2]);
			emission_func = [emissive_color](double u_, double v_, const point3& p_)->color {
				return fixed_color(u_, v_, p_, emissive_color);
			};
		}
		// the general case
		else
		{
			TexVec_func func = set_TexVecFunc(texture);
			emission_func = [func, emissive_factor](double u_, double v_, const point3& p_)->color {
				color output;
				vec4 func_out = func(u_, v_, 0.0, 0.0);
				output[0] = emissive_factor[0] * func_out[0];
				output[1] = emissive_factor[1] * func_out[1];
				output[2] = emissive_factor[2] * func_out[2];
				return output;
			};
		}
	}

	// setting the albedo function
	{
		const tg3_pbr_metallic_roughness& pmr = prop.pbr_metallic_roughness;
		const double* base_color_src = pmr.base_color_factor;
		double base_color[4];
		std::copy_n(base_color_src, 4, base_color);
		const tg3_texture_info* texture = &pmr.base_color_texture;
		double base_color_size = std::sqrt(
			base_color[0] * base_color[0] +
			base_color[1] * base_color[1] +
			base_color[2] * base_color[2] +
			base_color[3] * base_color[3]);
		// if it is not metallic at all 
		if (base_color_size < eps)
		{
			albedo_func = null_vec4;
		}
		// the texture is missing
		else if (texture->index == -1)
		{
			albedo_func = [base_color]
			(const double& u_, const double& v_, const double& u1_, const double& v1_)->vec4
			{
				return vec4{base_color[0],base_color[1],base_color[2],base_color[3]};
			};
		}
		// the general case
		else
		{
			TexVec_func func = set_TexVecFunc(texture);
			albedo_func = [func, base_color]
			(const double& u_, const double& v_, const double& u1_, const double& v1_)->vec4
			{
				vec4 output = func(u_,v_,u1_,v1_);
				output[0] *= base_color[0];
				output[1] *= base_color[1];
				output[2] *= base_color[2];
				output[3] *= base_color[3];
				return output;
			};
		}
	}

	// setting the metallic roughness function
	{
		const tg3_pbr_metallic_roughness& pmr = prop.pbr_metallic_roughness;
		const tg3_texture_info* texture = &pmr.metallic_roughness_texture;
		// the texture is missing
		if (texture->index == -1)
		{
			metal_rough_func = unity_vec2;
		}
		// the general case
		else
		{
			TexVec_func func = set_TexVecFunc(texture);
			metal_rough_func = [func] (
				const double& u_,
				const double& v_,
				const double& u1_,
				const double& v1_)->vec2
			{
				vec4 func_output = func(u_, v_, u1_, v1_);
				vec2 output = { func_output[2],func_output[1] };
				return output;
			};

		}
	}

	// the functional to get vec4 from the image file
	{
		// for each texture it is individually set by the set_TexVecFunc 
		// functional
	}
}

void PBR::scatter(const ray& r_in, const hit_record& rec, std::array<scatter_record, 3>& srec_, double& pdf_) const
{
	// transmitting ray 
	srec_[2] = { ray(rec.p, vec3(0,0,0), r_in.time()), color(0, 0, 0), 0.05,false };


	// getting u, v coordinates
	double u = rec.u, v = rec.v;
	double u1 = rec.u1, v1 = rec.v1;

	// getting the albedo from the base_color and the base_color_texture
	vec4 albedo_vec4 = albedo_func(u, v, u1, v1);
	vec3 albedo = { albedo_vec4[0],albedo_vec4[1],albedo_vec4[2] };


	// reflected part
	// geting the metallic_weight and the roughness
	vec2 metal_rough =  metal_rough_func(u, v, u1, v1);
	double metallic_weight = metal_rough[0];
	double roughness = metal_rough[1];


	// metallic 
	vec3 reflected = reflect(r_in.direction(), rec.normal);
	double metallic_factor = prop.pbr_metallic_roughness.metallic_factor;
	metallic_weight *= metallic_factor;
	metallic_weight = std::clamp(metallic_weight, 0.0, 1.0);
	double diffuse_weight = std::clamp(1.0 - metallic_weight, 0.0, 1.0);

	metallic_weight *= 0.95;
	diffuse_weight *= 0.95;



	// roughness 
	vec3 random_vector = random_unit_vector();
	double roughness_factor = prop.pbr_metallic_roughness.roughness_factor;
	vec3 roughReflected = roughness *
		vec3{ random_vector[0],
			  random_vector[1],
			  random_vector[2] };

	reflected += roughness_factor * roughReflected;
	reflected = unit_vector(reflected);

	srec_[1] = { ray(rec.p, reflected, r_in.time()), albedo, metallic_weight, true };


	// diffusive part
	vec3 diffuse = random_unit_vector() + rec.normal;
	diffuse = unit_vector(diffuse);
	srec_[0] = { ray(rec.p,diffuse,r_in.time()),albedo,diffuse_weight,true };
}

color PBR::emitted(const ray& r_in_, const hit_record& rec_, double _u, double _v, const point3& _p)  const
{
	if (!rec_.front_face)
		return color{ 0,0,0 };
	color clr = emission_func(_u, _v, _p);
	return clr;
}

bool PBR::is_equal(const material& _second) const
{
	return false;
}



void PBR::repeat_sampler(int& i_, const int size_)
{
	i_ = ((i_ % size_) + size_) % size_;
}

void PBR::mirror_sampler(int& i_, const int size_)
{
	i_ = i_ >= 0 ? i_ : i_ + size_;
	i_ = i_ % size_;
}

void PBR::mirror_repeat_sampler(int& i_, const int size_)
{
	int period = 2 * size_;
	i_ = ((i_ % period) + period) % period;

	if (i_ >= size_)
		i_ = period - 1 - i_;
}

void PBR::clamped_sampler(int& i_, const int size_)
{
	i_ = i_ < 0 ? 0 : i_;
	i_ = i_ >= size_ ? size_ - 1 : i_;
}

TexVec_func PBR::set_TexVecFunc(const tg3_texture_info* texture_)
{
	int32_t index = texture_->index;
	int32_t coord = texture_->tex_coord;
	tg3_image_result* img;
	/*if (coord != 0)
	{
		std::cout << "Warning: currently coords higher than 0 is not supported!";
	}*/

	// there is no texture
	if (index == -1)
	{
		return null_vec4;
	}
	// out of range
	else if (index >= resources->textures.size())
	{
		throw std::out_of_range("out of range access for the textures array");
	}
	tg3_texture& texture = resources->textures[index];
	int32_t source = texture.source;
	int32_t samplerIndex = texture.sampler;

	// no source
	if (source == -1)
	{
		return null_vec4;
	}
	// out of range
	else if (source >= resources->images.size())
	{
		throw std::out_of_range("out of range access for the images array");
	}

	//the image
	img = &resources->images[source];
	int width = img->width;
	int height = img->height;

	// dealing with the sampler function
	Sampler_func smpler_x, smpler_y;

	smpler_x = [width](int& i_) -> void
	{
		repeat_sampler(i_, width);
	};
	smpler_y = [height](int& i_)-> void
	{
		repeat_sampler(i_, height);
	};

	if (samplerIndex == -1)
	{
		smpler_x = [width](int& i_)->void
		{
			return repeat_sampler(i_, width);
		};
		smpler_y = [height](int& i_)->void
		{
			return repeat_sampler(i_, height);
		};
	}
	else if (samplerIndex >= resources->samplers.size())
	{
		throw std::out_of_range("out of range access for the samplers array");
	}
	else
	{
		tg3_sampler* smplr = &resources->samplers[samplerIndex];

		switch (smplr->wrap_s)
		{
		case TG3_TEXTURE_WRAP_REPEAT:
			smpler_x = [width](int& i_)->void
				{
					return repeat_sampler(i_, width);
				};
			break;
		case TG3_TEXTURE_WRAP_MIRRORED_REPEAT:
			smpler_x = [width](int& i_)->void
				{
					return mirror_repeat_sampler(i_, width);
				};
			break;
		case TG3_TEXTURE_WRAP_CLAMP_TO_EDGE:
			smpler_x = [width](int& i_)->void
				{
					return clamped_sampler(i_, width);
				};
			break;
		default:
			throw std::invalid_argument("Unsupported sampler!");
		}

		switch (smplr->wrap_t)
		{
		case TG3_TEXTURE_WRAP_REPEAT:
			smpler_y = [height](int& j_)->void
				{
					return repeat_sampler(j_, height);
				};
			break;
		case TG3_TEXTURE_WRAP_MIRRORED_REPEAT:
			smpler_y = [height](int& j_)->void
				{
					return mirror_repeat_sampler(j_, height);
				};
			break;
		case TG3_TEXTURE_WRAP_CLAMP_TO_EDGE:
			smpler_y = [height](int& j_)->void
				{
					return clamped_sampler(j_, height);
				};
			break;
		default:
			throw std::invalid_argument("Unsupported sampler!");
		}
	}


	TexVec_func func;
	PixelVec_func pixel_to_vec;
	std::array<Sampler_func, 2> smp = { smpler_x,smpler_y };
	// getting the image properties to choose appropriate functionals
	int32_t bits = img->bits;
	int32_t bytes = bits / 8;
	int32_t cpp = img->component;

	// selecting appropriate functionals
	switch (bytes)
	{
	case 1:
		switch (cpp)
		{
		case 1:
			pixel_to_vec = &image_to_vec4_1byte_1;
			break;
		case 2:
			pixel_to_vec = &image_to_vec4_1byte_2;
			break;
		case 3:
			pixel_to_vec = &image_to_vec4_1byte_3;
			break;
		case 4:
			pixel_to_vec = &image_to_vec4_1byte_3;
			break;
		default:
			throw std::invalid_argument("Unsupported image format");
		}
		break;
	case 2:
		switch (cpp)
		{
		case 1:
			pixel_to_vec = &image_to_vec4_2bytes_1;
			break;
		case 2:
			pixel_to_vec = &image_to_vec4_2bytes_2;
			break;
		case 3:
			pixel_to_vec = &image_to_vec4_2bytes_3;
			break;
		case 4:
			pixel_to_vec = &image_to_vec4_2bytes_3;
			break;
		default:
			throw std::invalid_argument("Unsupported image format");
		}
		break;
	default:
		throw std::invalid_argument("Error in parsing the image!");
	}
	if (coord == 0)
	{
		func = [img, smp, pixel_to_vec](double u_, double v_, double u1_, double v1_) ->vec4 {

			return tg3_image_to_color(u_, v_, img, smp, pixel_to_vec);
		};
	}
	else if (coord == 1)
	{
		func = [img, smp, pixel_to_vec](double u_, double v_, double u1_, double v1_) ->vec4 {
			return tg3_image_to_color(u1_, v1_, img, smp, pixel_to_vec);
		};
	}
	else
	{
		throw std::invalid_argument("Coords higher than 1 are not yet supported!");
	}
	return func;
}

vec4 PBR::tg3_image_to_color(
	double s_, double t_,
	const tg3_image_result* img_,
	const std::array<Sampler_func, 2> smp_,
	const PixelVec_func& pixel_to_vec_)
{
	int x = static_cast<int>((s_) * (img_->width - 1));
	int y = static_cast<int>((t_) * (img_->height - 1));
	//clamping the x and y 
	smp_[0](x);
	smp_[1](y);


	int32_t bits = img_->bits;
	int32_t bytes = bits / 8;
	int32_t cpp = img_->component;
	int32_t row = cpp * img_->width;

	int base = y * row + x * cpp;

	return pixel_to_vec_(img_->pixels, base);
}


vec4 PBR::image_to_vec4_1byte_1(uint8_t* pixels_, int& base_)
{
	constexpr int maxColor = 255.0;
	double g = pixels_[base_] / maxColor;
	return vec4{ g,g,g,1.0 };
}

vec4 PBR::image_to_vec4_1byte_2(uint8_t* pixels_, int& base_)
{
	constexpr int maxColor = 255.0;
	double g = pixels_[base_] / maxColor;
	double a = pixels_[base_ + 1] / maxColor;
	return vec4{ g,g,g,a };
}

vec4 PBR::image_to_vec4_1byte_3(uint8_t* pixels_, int& base_)
{
	constexpr int maxColor = 255.0;
	double r = static_cast<double>(pixels_[base_]) / maxColor;
	double g = static_cast<double>(pixels_[base_ + 1]) / maxColor;
	double b = static_cast<double>(pixels_[base_ + 2]) / maxColor;
	return vec4{ r,g,b,1.0 };
}

vec4 PBR::image_to_vec4_1byte_4(uint8_t* pixels_, int& base_)
{
	constexpr int maxColor = 255.0;
	double r = static_cast<double>(pixels_[base_]) / maxColor;
	double g = static_cast<double>(pixels_[base_ + 1]) / maxColor;
	double b = static_cast<double>(pixels_[base_ + 2]) / maxColor;
	double a = static_cast<double>(pixels_[base_ + 3]) / maxColor;
	return vec4{ r,g,b,a };
}

vec4 PBR::image_to_vec4_2bytes_1(uint8_t* pixels_, int& base_)
{
	constexpr int maxColor = 65535.0;
	unsigned short* chimage = reinterpret_cast<unsigned short*>(pixels_);
	double g = chimage[base_] / maxColor;
	return vec4{ g,g,g,1.0 };
}

vec4 PBR::image_to_vec4_2bytes_2(uint8_t* pixels_, int& base_)
{
	constexpr int maxColor = 65535.0;
	unsigned short* chimage = reinterpret_cast<unsigned short*>(pixels_);
	double g = chimage[base_] / maxColor;
	double a = chimage[base_+1] / maxColor;
	return vec4{ g,g,g,a };
}

vec4 PBR::image_to_vec4_2bytes_3(uint8_t* pixels_, int& base_)
{
	constexpr int maxColor = 65535.0;
	unsigned short* chimage = reinterpret_cast<unsigned short*>(pixels_);
	double r = chimage[base_] / maxColor;
	double g = chimage[base_ + 1] / maxColor;
	double b = chimage[base_ + 2] / maxColor;
	return vec4{ r,g,b,1.0 };
}

vec4 PBR::image_to_vec4_2bytes_4(uint8_t* pixels_, int& base_)
{
	constexpr int maxColor = 65535.0;
	unsigned short* chimage = reinterpret_cast<unsigned short*>(pixels_);
	double r = chimage[base_] / maxColor;
	double g = chimage[base_ + 1] / maxColor;
	double b = chimage[base_ + 2] / maxColor;
	double a = chimage[base_ + 3] / maxColor;
	return vec4{ r,g,b,a };
}

