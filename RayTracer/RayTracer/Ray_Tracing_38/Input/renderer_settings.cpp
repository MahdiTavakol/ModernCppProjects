#include "renderer_settings.h"
#include <fstream>
#include <sstream>

constexpr double eps = 1e-8;

renderer_settings::renderer_settings(int mode_):
	settings{mode_}
{
	set_mode(mode_);
}

void renderer_settings::set_input_map()
{
	singleInputMap = {
		// type of renderer
		{"-renderer_mode",&renderer_mode_str},

		{"-path_type",&pth_type_str},

		// circular path parameters
		{"-radius",&radius},
		{"-theta",&theta},

		// file path parameters
		{"-file",&file_name},


		// movie settings 
		{"-num_seconds",&num_seconds},
		{"-fps",&fps},
		{"--t",&num_seconds},
		{"--f",&fps}
	};

	threeInputMap = {
		// circular path parameters
		{"-center",std::tuple(&center[0],&center[1],&center[2])},
	};
}

void renderer_settings::set_mode(int mode_)
{
	mode = mode_;

	switch (mode_)
	{
	// static modes
	case QUADS:
		this->location = point3(0, 0, 9);
		break;
	case TWO_LIGHTS:
	case SIMPLE_LIGHT:
		this->location = point3(26, 3, 6);
		break;
	case CORNELL_BOX:
	case TWO_BOXES:
	case TWO_BOXES_ROTATED:
		this->location = point3(278, 278, -800);
		break;
	case CORNELL_SMOKE:
		this->location = point3(278, 278, -800);
		break;
	case SIMPLE_2D_PARALEL_TEST:
		this->location = point3(100, 100, 500);
		break;


	// animated modes
	case RANDOM_SPHERES_ANIMATED:
		this->fps = 60;
		this->num_seconds = 10;
		// Creating the trajectory of the camera
		center = point3{ 13, 2, 3 };
		radius = 13.49;
		theta = 45.0;
		break;


	default:
		throw std::invalid_argument("Wrong mode!");
	}
}

void renderer_settings::return_movie_params(int& num_seconds_, int& fps_)
{
	num_seconds = num_seconds_;
	fps = fps_;
}

void renderer_settings::return_path_type(Path_type& pth_type_)
{
	pth_type_ = pth_type;
}

void renderer_settings::return_location_param(point3 location_)
{
	location_ = location;
}

void renderer_settings::return_circular_params(point3& center_, double& radius_, double& theta_)
{
	if (pth_type != Path_type::CIRCULAR)
		throw std::runtime_error("The path type is not circular");
	center_ = center;
	radius_ = radius;
	theta_ = theta;
}

void renderer_settings::return_file_params(std::vector<point3_animated>& centers_)
{
	if (pth_type != Path_type::FILE)
		throw std::runtime_error("The path type is not circular");

	centers_ = std::move(centers);
}

void renderer_settings::extra_parse()
{
	// setting the renderer_mode
	if (renderer_mode_str == "STATIC")
	{
		render_mode = renderMode::STATIC;
	}
	else if (renderer_mode_str == "ANIMATION")
	{
		render_mode = renderMode::ANIMATION;
	}
	else
	{
		throw std::invalid_argument("Wrong render mode keyword!");
	}

	// setting the path_type if it exists
	if (pth_type == Path_type::FILE)
	{
		std::string line;
		double second;
		vec3 point;
		std::fstream file(file_name,std::ios::in);

		if (!file.is_open())
		{
			std::string error_text = "Cannot open file " + file_name;
			throw std::invalid_argument(error_text.c_str());
		}

		while (std::getline(file, line))
		{
			std::stringstream iss(line);
			if (iss >> second >> point)
			{
				std::clog << "Reading the second " << second << " from the file " << file_name << std::endl;
				point3_animated loc(point, second);
				centers.push_back(loc);
			}
			else
			{
				std::string error_text = "The format of the path file should be \"second x y z\" ";
				throw std::invalid_argument(error_text.c_str());
			}
		}
		std::clog << "Finished reading the file " << file_name << std::endl;
		file.close();

	}
}

void renderer_settings::check_validity() const
{
	if (render_mode != renderMode::ANIMATION)
	{
		if (pth_type != Path_type::NONE)
			throw std::invalid_argument("The path is just defined for the animation mode");
	}

	if (pth_type != Path_type::FILE)
	{
		if (file_name.empty())
			throw std::invalid_argument("File name has been specified for a path type which is not of file type!");
	}

	if (pth_type == Path_type::CIRCULAR)
	{
		if (radius <= eps)
			throw std::invalid_argument("The raidus for the circular path must be positive");
	}
}

void renderer_settings::log_class_name(std::iostream& stream_) const
{
	stream_ << "Path class options:" << std::endl << std::endl;
}