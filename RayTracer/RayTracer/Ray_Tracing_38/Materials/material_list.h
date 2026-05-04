#pragma once
#include "material.h"

#include <vector>
#include <memory>
#include <unordered_map>

class material_list {
public:
	material_list(int nmax_ = 10) :
		nmax{ nmax_ }
	{
		list.reserve(nmax);
	}

	void push_back(std::string name_, std::unique_ptr<material> mat_)
	{
		int indx = list.size();
		list.push_back(std::move(mat_));
		matMap[name_] = indx;
	}

	material* operator()(const int& indx_) const
	{
		return list[indx_].get();
	}

	material* operator()(const std::string name_)
	{
		return list[matMap[name_]].get();
	}
	
	size_t size() const
	{
		return list.size();
	}


private:
	int nmax;
	std::vector<std::unique_ptr<material>> list;
	std::unordered_map<std::string, int> matMap;

};
