#include "model_reader.h"
#include <fstream>
#include <filesystem>

model_reader::model_reader(std::string file_path_, communicator* para_)
	: 
	file_path(file_path_),
	para(para_),
	world{ std::make_unique<hittable_list>() },
	mtl_list{std::make_unique<material_list>()}
{
}

std::unique_ptr<hittable_list> model_reader::return_world()
{
	if (world == nullptr)
		throw std::invalid_argument("The world is empty!");
	return std::move(world);
}

std::unique_ptr<material_list> model_reader::return_mtl_list()
{
	if (mtl_list == nullptr)
		throw std::invalid_argument("The material list is empty!");
	return std::move(mtl_list);
}

std::unique_ptr<std::istream> model_reader::open_file(std::string& file_name_, std::string path_, bool silent_)
{
	char delimiter;
#ifdef _WIN32
	delimiter = '\\';
#else
	delimiter = '/';
#endif
	if (file_name_.empty())
		return nullptr;
	auto file_ptr =
		std::make_unique<std::ifstream>(file_name_);
	if (!file_ptr->is_open()) {
		file_name_ = path_ + file_name_;
		file_ptr = std::make_unique<std::ifstream>(file_name_);
		std::cout << "Trying opening " << file_name_ << std::endl;
		if (!file_ptr->is_open()) {
			if (!silent_)
				std::cout << "The file " << std::endl
				<< file_name_ << std::endl
				<< "does not exists in the path " << std::endl
				<< std::filesystem::current_path() << std::endl;
			return nullptr;
		}
	}
	return std::move(file_ptr);
}

void model_reader::set_range(int& _low, int& _hi)
{
	_low = 0;
	_hi = static_cast<int>(faces.size());
}

void model_reader::estimate_vns(
	const std::array<point3, 3>& vs_,
	std::array<point3, 3>& vns_)
{
	constexpr int num_edges = 3;
	for (int i = 0; i < num_edges; i++)
	{
		int prev = i ? i - 1 : num_edges - 1;
		int next = (i != num_edges - 1) ? i + 1 : 0;
		vec3 a = vs_[next] - vs_[i];
		vec3 b = vs_[prev] - vs_[i];
		vec3 normal = cross(a, b);
		vec3 unit_normal = unit_vector(normal);
		vns_[i] = unit_normal;
	}
}