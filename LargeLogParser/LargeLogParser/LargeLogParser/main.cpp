#include "Parser_NS/LogParser.h"
#include "Parser_NS/LogParserCreaterConc.h"


#include <iostream>
#include <memory>

void main(int narg, char** argv)
{
	using Parser_NS::LogParser;
	using Parser_NS::LogParserCreaterConc;

	std::vector<std::string> args;
	for (int i = 1; i < narg; i++)
		args.emplace_back(argv[i]);


	std::unique_ptr<LogParser> logParserPtr;
	LogParserCreaterConc parserCreater{ args };

	logParserPtr = std::move(parserCreater());

	logParserPtr->readFile();

	std::array<int, 4> nums;
	logParserPtr->returnNums(nums);

	std::cout << "Total number of lines: " << nums[0] << std::endl;
	std::cout << "Number of [INFO] lines: " << nums[1] << std::endl;
	std::cout << "Number of [WARN] lines: " << nums[2] << std::endl;
	std::cout << "Number of [ERROR] lines: " << nums[3] << std::endl;

	logParserPtr->writeData();

}