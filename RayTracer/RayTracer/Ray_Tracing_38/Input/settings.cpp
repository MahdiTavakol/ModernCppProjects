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
			auto iter3 = threeInputMap.find(text);
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
			else if (iter3 != threeInputMap.end())
			{
				auto& val = iter3->second;
				if (val.index() == 0)
				{
					std::tuple<int*, int*,int*> rawPair = std::get<std::tuple<int*, int*,int*>>(val);
					int* first  = std::get<0>(rawPair);
					int* second = std::get<1>(rawPair);
					int* third  = std::get<2>(rawPair);
					parse_input<int>(iss, *first, *second, *third);
				}
				else if (val.index() == 1)
				{
					std::tuple<double*, double*, double*> rawPair = std::get<std::tuple<double*, double*, double*>>(val);
					double* first = std::get<0>(rawPair);
					double* second = std::get<1>(rawPair);
					double* third = std::get<2>(rawPair);
					parse_input<double>(iss, *first, *second, *third);
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

void settings::log_class_name(std::iostream& stream_) const
{
	stream_ << "Settings class options:" << std::endl << std::endl;
}

void settings::logger(std::iostream& stream_) const
{
	// the header for the log file
	log_class_name(stream_);
	for (auto& pair : singleInputMap)
	{
		auto& val = pair.second;
		if (val.index() == 0)
		{
			int* ptr = std::get<int*>(val);
			stream_ << pair.first << " " << *ptr << std::endl;
		}
		else if (val.index() == 1)
		{
			double* ptr = std::get<double*>(val);
			stream_ << pair.first << " " << *ptr << std::endl;
		}
		else if (val.index() == 2)
		{
			std::string* ptr = std::get<2>(val);
			stream_ << pair.first << " " << *ptr << std::endl;
		}
	}
	
	
	for (auto& pair : doubleInputMap)
	{
		auto& val = pair.second;
		if (val.index() == 0)
		{
			std::pair<int*,int*> value = std::get<std::pair<int*, int*>>(val);
			int value1 = *(value.first);
			int value2 = *(value.second);
			stream_ << pair.first << " " << value1 << " " << value2 << std::endl;
		}
		else if (val.index() == 1)
		{
			std::pair<double*, double*> value = std::get<std::pair<double*, double*>>(val);
			double value1 = *(value.first);
			double value2 = *(value.second);
			stream_ << pair.first << " " << value1 << " " << value2 << std::endl;
		}
		else if (val.index() == 2)
		{
			std::pair<std::string*, std::string*> value = std::get<std::pair<std::string*, std::string*>>(val);
			std::string value1 = *(value.first);
			std::string value2 = *(value.second);
			stream_ << pair.first << " " << value1 << " " << value2 << std::endl;
		}
	}

	for (auto& pair : threeInputMap)
	{
		auto& val = pair.second;
		if (val.index() == 0)
		{
			std::tuple<int*, int*,int*> value = std::get<std::tuple<int*, int*, int*>>(val);
			int value1 = *(std::get<0>(value));
			int value2 = *(std::get<1>(value));
			int value3 = *(std::get<2>(value));
			stream_ << pair.first << " " << value1 << " " << value2 << " " << value3 << std::endl;
		}
		else if (val.index() == 1)
		{
			std::tuple<double*, double*, double*> value = std::get<std::tuple<double*, double*, double*>>(val);
			double value1 = *(std::get<0>(value));
			double value2 = *(std::get<1>(value));
			double value3 = *(std::get<2>(value));
			stream_ << pair.first << " " << value1 << " " << value2 << " " << value3 << std::endl;
		}
		else if (val.index() == 2)
		{
			// not implemented yet!
		}
	}

	stream_ << std::endl << std::endl << std::endl;


}