#pragma once
#include <string>
#include <vector>
#include <iostream>

class Logger {
public:
	Logger();

	Logger(const std::vector<std::reference_wrapper<std::ostream>>& strms_):
		strms{strms_}
	{}

	void print(std::string& text) const;

private:
	std::vector<std::reference_wrapper<std::ostream>> strms;


};