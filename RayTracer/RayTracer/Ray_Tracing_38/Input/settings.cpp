#include "settings.h"


void settings::parse_commands()
{
	for (auto& cmd : commands)
	{
		std::istringstream iss(cmd);
		std::string text;

		while (iss >> text) {
			auto iter = singleInputMap.find(text);
			auto iter2 = doubleInputMap.find(text);
			if (iter != singleInputMap.end())
			{
				auto& val = iter->second;
				if (val.index() == 0)
				{
					int* raw = std::get<int*>(val);
					parse_input<int>(iss, *raw);
				}
				else if (val.index() == 1)
				{
					double* raw = std::get<double*>(val);
					parse_input<double>(iss, *raw);
				}
				else if (val.index() == 2)
				{
					std::string* raw = std::get<std::string*>(val);
					parse_input<std::string>(iss, *raw);
				}
			}
			else if (iter2 != doubleInputMap.end())
			{
				auto& val = iter2->second;
				if (val.index() == 0)
				{
					std::pair<int*, int*> rawPair = std::get<std::pair<int*, int*>>(val);
					int* first = rawPair.first;
					int* second = rawPair.second;
					parse_input<int>(iss, *first, *second);
				}
				else if (val.index() == 1)
				{
					std::pair<double*, double*> rawPair = std::get<std::pair<double*, double*>>(val);
					double* first = rawPair.first;
					double* second = rawPair.second;
					parse_input<double>(iss, *first, *second);
				}
			}
		}
		
	}
}

void settings::check_validity() const
{
	if (mode < 0)
		throw std::invalid_argument("Wrong mode value");
}