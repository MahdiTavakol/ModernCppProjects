#pragma once
#include "material.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include "../Input/tiny_gltf_v3.h"

class material_list {
public:
	material_list(int nmax_ = 10);

	// releasing the static resources for each material type
	~material_list();

	int push_back(std::string name_, std::unique_ptr<material> mat_);
	int push_back(std::pair<std::string, std::unique_ptr<material>> in_);

	material* operator()(const int& indx_) const;
	material* operator()(const std::string name_);

	int find(const std::string name_);

	
	size_t size() const;

	void replace_material(std::string name_, std::unique_ptr<material> mat_);


private:
	int nmax;
	std::vector<std::unique_ptr<material>> list;
	std::unordered_map<std::string, int> matMap;

};
