#pragma once
#include <string>
#include <iostream>

using std::cout;
using std::endl;

class Logger
{
public:
	const void operator()(std::string& input_)
	{
		cout << input_ << endl;
	}
	const void operator()(const char* input_)
	{
		cout << input_ << endl;
	}
};