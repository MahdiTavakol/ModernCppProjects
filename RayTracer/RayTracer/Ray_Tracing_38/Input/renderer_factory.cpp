#include "renderer_factory.hpp"

#include "../Renderer/Renderer_Animation.h"

renderer_factory::renderer_factory(settings* stngs_, communicator* para_) :
	para{ para_ }
{
	// checking the setting type
	renderer_settings* render_sett = dynamic_cast<renderer_settings*>(stngs_);
	if (!render_sett)
		throw std::invalid_argument("Wrong settings object");


	// getting the path_type
	render_sett->return_path_type(pth_type);
	
	// checking the render mode
	render_sett->return_render_mode(render_mode);


	switch (render_mode)
	{
		case renderMode::STATIC:
		{
			// getting the camera location
			render_sett->return_location_param(location);
			break;
		}
		case renderMode::ANIMATION:
		{
			// getting movie parameters
			render_sett->return_movie_params(num_seconds, fps);
			// getting the path parameters
			render_sett->return_path_type(pth_type);
			// checking the path type
			if (pth_type == Path_type::NONE)
				throw std::invalid_argument("There must be a path for the animation rendering mode!");

			if (pth_type == Path_type::CIRCULAR)
			{
				render_sett->return_circular_params(center, radius, theta);
			}
			else if (pth_type == Path_type::FILE)
			{
				render_sett->return_file_params(centers);
			}
			else
			{
				throw std::invalid_argument("Invalid path type");
			}
		}
		default:
			throw std::invalid_argument("Unrecoverable error!");
	}
}

void renderer_factory::create()
{
	std::unique_ptr<path> pth = this->create_and_return_path();
	switch (render_mode)
	{
	case renderMode::STATIC:
		renderObj = std::make_unique<renderer>(para,std::move(pth));
		break;
	case renderMode::ANIMATION:
		renderObj = std::make_unique<renderer_animation>(para,std::move(pth));
		break;
	default:
		throw std::invalid_argument("Unknown rendering mode");
	}
}

std::unique_ptr<path> renderer_factory::create_and_return_path()
{
	switch (pth_type)
	{
	case Path_type::NONE:
		return std::make_unique<path>(location);
	case Path_type::CIRCULAR:
		return std::make_unique<path>(center, radius, theta, num_seconds, fps);
	case Path_type::FILE:
		return std::make_unique<path>(std::move(centers), num_seconds, fps);
	}
	return nullptr;
}

std::unique_ptr<renderer> renderer_factory::return_object()
{
	if (!renderObj)
	{
		std::cout << "The renderer object has already been returned\nCreating it again!" << std::endl;
		create();
	}
	return std::move(renderObj);
}