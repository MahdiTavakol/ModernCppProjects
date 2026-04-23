#include "app_settings.h"


app_settings::app_settings(
	const int mode_,
	std::vector<std::unique_ptr<settings>>&& settingsVec_,
	std::map<std::string, int> settingsMap_) :
	settings{ mode_ },
	settingsMap{settingsMap_},
	settingsVec{ std::move(settingsVec_) }
{}

void app_settings::set_mode(int mode_)
{
	for (auto& set : settingsVec)
	{
		set->set_mode(mode_);
	}
}

void app_settings::push_back(std::unique_ptr<settings>&& item_)
{
	settingsVec.push_back(std::move(item_));
}

settings* app_settings::return_settings(const std::string name_) const
{
	auto iter = settingsMap.find(name_);
	if (iter == settingsMap.end())
		return nullptr;
	int indx = iter->second;
	if (indx >= settingsVec.size())
		throw std::runtime_error("Internal error!");
	return settingsVec[indx].get();
}


void app_settings::check_validity() const
{
	if (mode < 0)
		throw std::invalid_argument("The mode cannot be negative!");

	for (auto& sett : settingsVec)
	{
		sett->check_validity();
	}
}