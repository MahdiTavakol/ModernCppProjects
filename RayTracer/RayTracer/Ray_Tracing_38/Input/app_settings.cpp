#include "app_settings.h"


app_settings::app_settings(
	const int mode_,
	std::vector<std::unique_ptr<settings>>&& settingsVec_,
	std::map<std::string, int> settingsMap_) :
	settings{ mode_ },
	settingsMap{settingsMap_},
	settingsVec{ std::move(settingsVec_) }
{
	// setting the default parameters based on the chosen mode
	this->set_mode(mode_);
}


void app_settings::set_mode(int mode_)
{
	for (auto& set : settingsVec)
	{
		set->set_mode(mode_);
	}
}

bool app_settings::add_cmd(std::string indx_, std::string cmd_)
{
	auto iter = settingsMap.find(indx_);
	if (iter != settingsMap.end())
	{
		int indx = iter->second;
		settingsVec[indx]->add_cmd(cmd_);
		return true;
	}
	return false;
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
	settings::check_validity();

	for (auto& sett : settingsVec)
	{
		sett->check_validity();
	}
}

void app_settings::logger(std::iostream& stream_) const {
	for (auto& sett : settingsVec)
	{
		sett->logger(stream_);
	}
}

settings* app_settings::operator[](const std::string& text_)
{
	auto iter = settingsMap.find(text_);
	if (iter == settingsMap.end()) {
		std::string error_str = "There is no variable " + std::string(text_) + " in the app_settings!";
		throw std::runtime_error(error_str.c_str());
		return nullptr;
	}
	return settingsVec[iter->second].get();
}

void app_settings::parse_commands()
{
	for (auto& sett : settingsVec)
	{
		sett->parse_commands();
	}
}