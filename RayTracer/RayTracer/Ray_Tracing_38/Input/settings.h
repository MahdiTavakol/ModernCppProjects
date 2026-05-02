#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <variant>
#include <concepts>
#include <map>

#include "../Shared/rtweekend.h"


using singleVar = std::variant<int*, double*, std::string*>;
using doubleVar = std::variant<std::pair<int*, int*>, std::pair<double*, double*>,std::pair<std::string*,std::string*>>;
using threeVar = std::variant<std::tuple<int*, int*, int*>, std::tuple<double*, double*, double*>>;

template<typename T>
concept AllowedTypes =
	std::same_as<T, int> ||
	std::same_as<T, double> ||
	std::same_as<T, float>;

class settings
{
public:
	settings() = default;
	settings(int mode_) : mode{ mode_ } {}
	virtual void set_input_map() = 0;
	int return_mode() { return mode; }
	void add_cmd(const std::string& cmd_) { commands.push_back(cmd_); }
	virtual void set_mode(int mode_) { mode = mode_; }
	virtual void check_validity() const;
	virtual void parse_commands();
	virtual void extra_parse() {}
	virtual void log_class_name(std::iostream& stream_) const;
	virtual void logger(std::iostream& stream_) const;



	template<AllowedTypes T>
	T convert_string(std::string str_)
	{
		T _t(0);
		std::stringstream iss(str_);

		if (str_.empty())
		{
			throw std::invalid_argument("Null point passed");
		}

		iss >> _t;

		// Check if the conversion was successful
		if (iss.fail()) {
			throw std::invalid_argument("Conversion failed");
		}

		return _t;
	}

	// parsing parameters
	template<AllowedTypes T>
	void parse_input(std::istringstream& iss_,  T& param_)
	{
		std::string text;
		if (iss_ >> text)
		{
			param_ = convert_string<T>(text);
		}
		else
		{
			std::cerr << "Invalid input arguments" << std::endl;
		}
	}


	template<AllowedTypes T>
	void parse_input(std::istringstream& iss_,T& param1_, T& param2_)
	{
		std::string text1, text2;
		if (iss_ >> text1 >> text2)
		{
			param1_ = convert_string<T>(text1);
			param2_ = convert_string<T>(text2);
		}
		else
		{
			std::cerr << "Invalid input arguments" << std::endl;
		}
	}

	template<AllowedTypes T>
	void parse_input(std::istringstream& iss_, T& param1_, T& param2_, T& param3_)
	{
		std::string text1, text2, text3;
		if (iss_ >> text1 >> text2 >> text3)
		{
			param1_ = convert_string<T>(text1);
			param2_ = convert_string<T>(text2);
			param3_ = convert_string<T>(text3);
		}
		else
		{
			std::cerr << "Invalid input arguments" << std::endl;
		}
	}

protected:
	int mode = 0;
	std::vector<std::string> commands;


	std::map<std::string, singleVar> singleInputMap;
	std::map<std::string, doubleVar> doubleInputMap;
	std::map<std::string, threeVar> threeInputMap;


};