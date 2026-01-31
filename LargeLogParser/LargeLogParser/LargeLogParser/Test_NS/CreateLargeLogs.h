#pragma once

#include "fileWriter.h"

#include <string>
#include <memory>

namespace Test_NS {


	class CreateLargeLogs
	{
	public:
		CreateLargeLogs(const std::string& logFileName_, const int& num_lines_, const int& line_length_,
			const double& info_percent_, const double& warns_percent_, const double& errors_percent_,
			const bool& keep_temp_);

		CreateLargeLogs(std::unique_ptr<fileWriter>&& fileWriterPtr_) :
			fileWriterPtr{ std::move(fileWriterPtr_) } {
		}

		~CreateLargeLogs();

		void generateLog();
		int operator()(std::array<int, 3>& nums_);


	private:
		// file 
		std::unique_ptr<fileWriter> fileWriterPtr;

		bool keep_temp = true;
	};

} // namespace Test_NS
