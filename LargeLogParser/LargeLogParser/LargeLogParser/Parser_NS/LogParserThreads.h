#pragma once


#include "../Data_NS/DataStructure.h"
#include "LogParser.h"

#include <string>
#include <thread>
#include <vector>


namespace Parser_NS {
	class LogParserThreads final : public LogParser
	{

	public:
		LogParserThreads(std::string filePath_, const int& numThreads_, bool silentMode_);
		LogParserThreads(std::string filePath_, const int& numThreads_, const int& beg_, const int& end_, bool silentMode_);
		~LogParserThreads() = default;
		void readFile() override;

		/*void printProgress(const double& progress) override
		{
			int id = 0;
			for (auto& logParser : logParsers) {
				//std::lock_guard<std::mutex> lock(cout_mutex);
				//std::cout << std::format("[Thread {}] Progress: {:.2f}%", id++, progress) << std::endl;
			}
		}*/

	private:
		int numThreads;
		std::mutex cout_mutex;

		std::vector<std::thread> threads;
		std::vector<LogParser> logParsers;

	};
} // namespace Parser_NS