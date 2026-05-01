#pragma once
#include "settings.h"

#include <vector>
#include <map>


class app_settings: public settings
{
public:
	// this class cannot create the settingsVec itself so I have deleted the default constructor
	app_settings() = delete;

	app_settings(const int mode_,
		std::vector<std::unique_ptr<settings>>&& settingsVec_,
		std::map<std::string, int> settingMap_);


	void set_mode(int mode_) override;

	void push_back(std::unique_ptr<settings>&& item);

	bool add_cmd(std::string indx_, std::string cmd_);


	settings* return_settings(const std::string name_) const;


	void check_validity() const;

	void parse_commands() override;

	void logger(std::iostream& stream_) const override;

	virtual void set_input_map() override {}

	settings* operator[](const int indx) {
		return settingsVec[indx].get();
	}

	settings* operator[](const std::string& text_);

protected:
	std::map<std::string, int> settingsMap;
	std::vector<std::unique_ptr<settings>> settingsVec;
};
