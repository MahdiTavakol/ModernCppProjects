#include "LogParserCreaterConc.h"

using namespace Parser_NS;

LogParserCreaterConc::LogParserCreaterConc(const std::vector<std::string>& args_):
	LogParserCreater()
{
	std::string filePath = "LogFile.log";
	int numThreads = 1;
	std::string parserType = "SERIAL";
	bool silentMode = false;

	int narg = args_.size();
	int iarg = 1;
	while (iarg < narg) {
		if (args_[iarg] == "--file" || args_[iarg] == "-f") {
			if (iarg + 1 >= narg)
				throw std::invalid_argument("Missing argument for --file");
			filePath = args_[iarg + 1];
			iarg += 2;
		}
		else if (args_[iarg] == "--threads" || args_[iarg] == "-t") {
			if (iarg + 1 >= narg)
				throw std::invalid_argument("Missing argument for --threads");
			numThreads = std::stoi(args_[iarg + 1]);
			iarg += 2;
		}
		else if (args_[iarg] == "--parser" || args_[iarg] == "-p") {
			if (iarg + 1 >= narg)
				throw std::invalid_argument("Missing argument for --parser");
			parserType = args_[iarg + 1];
			iarg += 2;
		}
		else if (args_[iarg] == "--silent" || args_[iarg] == "-s") {
			silentMode = true;
			iarg += 1;
		}
		else if (args_[iarg] == "--help" || args_[iarg] == "-h") {
			std::cout << "Usage: LogParser [options]\n"
				<< "Options:\n"
				<< "  --file, -f <file_path>       Specify the log file path (default: LogFile.log)\n"
				<< "  --threads, -t <num_threads>  Specify the number of threads (default: 1)\n"
				<< "  --parser, -p <type>          Specify the parser type (SERIAL, THREADS, FUTURE) (default: SERIAL)\n"
				<< "  --help, -h                   Show this help message\n";
			throw std::invalid_argument("Help requested");
		}
		else {
			throw std::invalid_argument("Unknown argument: " + args_[iarg]);
		}
	}


	if (parserType == "SERIAL") {
		logParserPtr = std::make_unique<LogParser>(filePath,silentMode);
	}
	else if (parserType == "THREADS") {
		logParserPtr = std::make_unique<LogParserThreads>(filePath, numThreads, silentMode);
	}
	else if (parserType == "FUTURE") {
		logParserPtr = std::make_unique<LogParserFuture>(filePath, numThreads, silentMode);
	}
 
}