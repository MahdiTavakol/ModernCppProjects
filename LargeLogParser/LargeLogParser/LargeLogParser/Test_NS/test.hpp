#pragma once
#include "CreateLargeLogs.h"
#include "../Parser_NS/LogParser.h"
#include "../Parser_NS/LogParserCreaterConc.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

enum class TestingMode
{
	TIMING,
	NO_TIMING
};


using namespace Test_NS;
using Parser_NS::LogParser;
using Parser_NS::LogParserCreaterConc;

template<TestingMode tMode>
double run_parser(std::unique_ptr<LogParser>& logParserPtr)
{
	if constexpr (tMode == TestingMode::TIMING)
	{
		auto t1 = high_resolution_clock::now();
		logParserPtr->readFile();
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		double duration = ms_double.count();
		return duration;
	}
	else if constexpr (tMode == TestingMode::NO_TIMING)
		logParserPtr->readFile();
	return 0.0;
}


template<TestingMode tMode>
void test_parser(std::string& logFileName, std::string parserArg, const std::array<int, 4>& expectedValues,
	const int& numThreads, double& timingInfo)
{
	std::array<int, 4> outputValues;
	std::unique_ptr<LogParser> logParserPtr;
	std::vector<std::string> args;

	args = { "LogParser", "--file", logFileName, "--parser", parserArg, 
		     "--threads", std::to_string(numThreads), "--silent" };

	LogParserCreaterConc logParserCreater(args);
	logParserPtr = std::move(logParserCreater());

	Data_NS::DataStructure outputData;
	timingInfo = run_parser<tMode>(logParserPtr);
	logParserPtr->returnLogs<Parser_NS::ReturnMode::MOVE>(outputData);
	outputData(outputValues);

	std::string failText = std::string("Failed for ") + logFileName;
	REQUIRE(expectedValues == outputValues);
	//REQUIRE_MESSAGE(expectedValues == outputValues,failText);
}

/*
 * The functions down below are for the cases when 
 * we ourselves create logs rather than reading the log*
 * whose number of warning, info and error lines we know 
 * in advance.
 */

void CreateLogs(
	const std::unique_ptr<fileInfo>& fileInfoPtr,
	std::array<int, 4>& expectedValues_)
{
	std::unique_ptr<fileWriter> fileWriterPtr =
		std::make_unique<fileWriterSimple>(fileInfoPtr);
	CreateLargeLogs logCreator{std::move(fileWriterPtr)};
	logCreator.generateLog();
	std::array<int,3> nums;
	// numlines
	expectedValues_[0] = logCreator(nums);
	// numinfos
	expectedValues_[1] = nums[0];
	// numwarnings
	expectedValues_[2] = nums[1];
	// numerrors
	expectedValues_[3] = nums[2];
}


template< TestingMode tMode>
void test_parser(std::unique_ptr<fileInfo>& fileInfoPtr_,
				 std::string parserArg,
                 double& timing_, const int& numThreads_)
{
	std::array<int,4> expectedValues;
	CreateLogs(fileInfoPtr_,expectedValues);
	std::string fileName = fileInfoPtr_->returnFileName();
	double& duration  = timing_;
	test_parser<tMode>(fileName,parserArg,expectedValues,numThreads_,duration);
}

template<TestingMode tMode>
void test_parser(std::vector<std::unique_ptr<fileInfo>>& fileInfoVec_,
	               std::string parserArg,
                   std::vector<double>& timings_, const int& numThreads_)
{
	timings_.resize(fileInfoVec_.size()); // The test_parser doesnot push_back so I need to initialize it with zeros
	int nData = static_cast<int>(fileInfoVec_.size());
	for (int i = 0; i < nData; i++) {
		test_parser<tMode>(fileInfoVec_[i],parserArg,timings_[i],numThreads_);
	}
}
