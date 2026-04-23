#pragma once
#include "settings.h"

#include <vector>
#include <map>

#include "camera_settings.h"
#include "image_settings.h"
#include "output_settings.h"
#include "world_settings.h"

class app_settings: public settings
{
public:
	// this class cannot create the settingsVec itself so I have deleted the default constructor
	app_settings() = delete;

	app_settings(const int mode_,
		std::vector<std::unique_ptr<settings>>&& settingsVec_,
		std::map<std::string, int> settingMap_);

	void set_mode(int mode_) override;

	void push_back(std::unique_ptr<settings>&& iterm);


	settings* return_settings(const std::string name_) const;

	renderMode& return_render_mode()
	{
		return renderMode;
	}

	void check_validity() const;

private:
	renderMode renderMode = renderMode::STATIC;

	std::map<std::string, int> settingsMap;
	std::vector<std::unique_ptr<settings>> settingsVec;
};
