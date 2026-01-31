#include "Parser_NS/LogParser.h"
#include "Parser_NS/LogParserCreaterConc.h"


#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
	using Parser_NS::LogParser;
	using Parser_NS::LogParserCreaterConc;

	std::vector<std::string> args;

	if (argc == 1) {
		std::cout << "Usage: LogParser [options]\n"
			<< "Options:\n"
			<< "  --file, -f <file_path>       Specify the log file path (default: LogFile.log)\n"
			<< "  --threads, -t <num_threads>  Specify the number of threads (default: 1)\n"
			<< "  --parser, -p <type>          Specify the parser type (SERIAL, THREADS, FUTURE) (default: SERIAL)\n"
			<< "  --help, -h                   Show this help message\n";
		std::cout << "Using default parameters." << std::endl;
	} else {
		for (int i = 1; i < argc; i++)
			args.emplace_back(argv[i]);
	}


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

	return 0;
}