#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <array>
#include <ios>
#include <iostream>
#include <mutex>
#include "../Data_NS/DataStructure.h"

namespace Parser_NS {
	using Data_NS::DataStructure;

	enum class ReturnMode
	{
		COPY,
		MOVE
	};

	class LogParser
	{
	public:
		LogParser(std::string filePath_, 
			bool silentMode = false, std::array<std::string, 3> keys = { "[ERROR]", "[WARN]", "[INFO]" });
		LogParser(std::string filePath_, const int& file_length_, int beg_, int end_, bool silentMode = true);
		LogParser(const LogParser&& rhs_) noexcept = default;
		virtual ~LogParser() {
			if (file.is_open())
				file.close();
		}
		virtual void readFile();
		/*void readFile(int& beg_, int& end_) {
			this->beg = beg_; this->end = end_;
			readFile();
		}*/
		virtual void printProgress(const double& progress);
		template<ReturnMode returnMode>
		void returnLogs(DataStructure& data_struct_)
		{
			switch (returnMode)
			{
			case ReturnMode::COPY:
				data_struct_ = data_struct;
				return;
			case ReturnMode::MOVE:
				data_struct_ = std::move(data_struct);
				return;
			default:
				throw std::invalid_argument("Wrong return mode!");
			}
		}
		void returnFileLength(int& fileLength_) { fileLength_ = fileLength; }
		void returnNums(std::array<int, 4>& nums_)
		{
			nums_[0] = data_struct.returnNumLines();
			auto dum = data_struct.returnNums();
			nums_[1] = dum[0]; nums_[2] = dum[1]; nums_[3] = dum[2];
		}

		void writeData();
		static int inquireFileLength(std::string filePath_);

	protected:
		// file info
		std::string filePath;
		std::ifstream file;
		int fileLength;

		// Should I print the progress?
		bool silentMode = true;

		// the reading range
		int beg, end;

		// log parsing keys
		const std::string error_key, warn_key, info_key;


		// data structure
		DataStructure data_struct;
	};
} // namespace Parser_NS
