#include "path_settings.h"
#include <fstream>
#include <sstream>

constexpr double eps = 1e-8;

path_settings::path_settings(int mode_):
	settings{mode_}
{}

void path_settings::set_input_map()
{
	singleInputMap = {
		{"-path_type",&pth_type_str},

		// circular path parameters
		{"-radius",&radius},
		{"-theta",&theta},

		// file path parameters
		{"-file",&file_name},


		// movie settings 
		{"-num_seconds",&num_seconds},
		{"-fps",&fps}
	};

	threeInputMap = {
		// circular path parameters
		{"-center",std::tuple(&center[0],&center[1],&center[2])},
	}
}

void path_settings::return_movie_params(int& num_seconds_, int& fps_)
{
	num_seconds = num_seconds_;
	fps = fps_;
}

void path_settings::return_path_type(Path_type& pth_type_)
{
	pth_type_ = pth_type;
}

void path_settings::return_circular_params(point3& center_, double& radius_, double& theta_)
{
	if (pth_type != Path_type::CIRCULAR)
		throw std::runtime_error("The path type is not circular");
	center_ = center;
	radius_ = radius;
	theta_ = theta;
}

void path_settings::return_file_params(std::vector<point3>& centers_)
{
	if (pth_type != Path_type::FILE)
		throw std::runtime_error("The path type is not circular");
}

void path_settings::extra_parse()
{
	if (pth_type == Path_type::FILE)
	{
		std::fstream file(file_name, std::ios::in);
		if (!file.is_open())
		{
			std::string error_text = "The file " + file_name + " specified for the path does not exists!";
			throw std::runtime_error(error_text.c_str());
		}
		int n_frames;
		std::string line;
		// the first line is comment (possibly the author name)
		std::getline(file, line);
		// the second line is also comment (possibly the file format)
		std::getline(file, line);
		// the third line contains the number of points
		{
			std::getline(file, line);
			std::stringstream iss(line);
			if (!(iss >> n_frames))
				throw std::runtime_error("Corrupted file format!");
			centers.reserve(n_frames);
		}
		// reading the frames
		int frames_read = 0;
		while (std::getline(file, line))
		{
			int frame_number;
			// reading a point3 from the file
			// and then pushing it into the centers 
			// vector is possible. However, it requirs
			// first creation of the point3 .. then reading the contents
			// from the file and finally copying it into the vector.
			// However, I want to read three number from the file
			// and then emplace_back into the vector to 
			// do the inplace construction
			double x, y, z;
			std::stringstream iss(line);
			if (!(iss >> frame_number >> x >> y>> z))
				throw std::runtime_error("Corrupted file format!");
			centers.emplace_back(x, y, z);
		}
	}
}

void path_settings::check_validity() const
{
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

void path_settings::log_class_name(std::iostream& stream_) const
{
	stream_ << "Path class options:" << std::endl << std::endl;
}