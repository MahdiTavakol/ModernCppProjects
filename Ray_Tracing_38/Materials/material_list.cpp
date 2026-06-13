#include "material_list.h"
#include "PBR.h"


material_list::material_list(int nmax_) :
	nmax{ nmax_ }
{
	list.reserve(nmax);
}

// releasing the static resources for each material type
material_list::~material_list()
{
}

int material_list::push_back(std::string name_, std::unique_ptr<material> mat_)
{
	int indx;
	auto iter = matMap.find(name_);
	if (iter == matMap.end())
	{
		list.push_back(std::move(mat_));
		indx = static_cast<int>(list.size()) - 1;
		matMap[name_] = indx;
	}
	else
	{
		indx = matMap[name_];
	}
	return indx;
}


int material_list::push_back(std::pair<std::string, std::unique_ptr<material>> in_)
{
	std::string name = in_.first;
	std::unique_ptr<material> mat = std::move(in_.second);

	return push_back(name, std::move(mat));
}

material* material_list::operator()(const int& indx_) const
{
	return list[indx_].get();
}

material* material_list::operator()(const std::string name_)
{
	return list[matMap[name_]].get();
}

int material_list::find(const std::string name_)
{
	auto iter = matMap.find(name_);
	if (iter != matMap.end())
		return matMap[name_];
	else
		return -1;
}


size_t material_list::size() const
{
	return list.size();
}

void material_list::replace_material(std::string name_, std::unique_ptr<material> mat_)
{
	int loc = find(name_);
	if (loc == -1)
	{
		std::string error_text = "material " + name_ + " was not found!";
		throw std::invalid_argument(error_text);

	}
	else
	{
		list[loc] = std::move(mat_);
	}
}