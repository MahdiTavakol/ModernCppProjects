#pragma once

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

enum class TestingMode
{
	TIMING,
	NO_TIMING
};

enum class ParserType
{
	SERIAL,
	THREADS,
	FUTURE
};


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


template<ParserType pType, TestingMode tMode>
void test_parser(std::string& logFileName, const std::array<int, 4>& expectedValues,
	const int& numThreads, double& timingInfo)
{
	std::array<int, 4> outputValues;
	std::unique_ptr<LogParser> logParserPtr;

	if constexpr (pType == ParserType::SERIAL) {
		// Serial log parser
		logParserPtr = std::make_unique<LogParser>(logFileName);
	}
	else if constexpr (pType == ParserType::THREADS) {
		// Threads log parser
		logParserPtr = std::make_unique<LogParserThreads>(logFileName,numThreads);
	}
	else if constexpr (pType == ParserType::FUTURE) {
		// Parallel log parser with future
		logParserPtr = std::make_unique<LogParserFuture>(logFileName,numThreads);
	}
	else
		static_assert(pType != pType,"Wrong argument!");

	DataStructure outputData;
	timingInfo = run_parser<tMode>(logParserPtr);
	logParserPtr->returnLogs<ReturnMode::MOVE>(outputData);
	outputData(outputValues);

	REQUIRE(expectedValues == outputValues);
}

/*
 * The functions down below are for the cases when 
 * we themselves create logs rather than reading the log*
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


template<ParserType pType, TestingMode tMode>
void test_parser(std::unique_ptr<fileInfo>& fileInfoPtr_, 
                 double& timing_, const int& numThreads_)
{
	std::array<int,4> expectedValues;
	CreateLogs(fileInfoPtr_,expectedValues);
	std::string fileName = fileInfoPtr_->returnFileName();
	double& duration  = timing_;
	test_parser<pType,tMode>(fileName,expectedValues,numThreads_,duration);
}

template<ParserType pType, TestingMode tMode>
void test_parser(std::vector<std::unique_ptr<fileInfo>>& fileInfoVec_,
                   std::vector<double>& timings_, const int& numThreads_)
{
	timings_.resize(fileInfoVec_.size()); // The test_parser doesnot push_back so I need to initialize it with zeros
	for (int i = 0; i < fileInfoVec_.size(); i++) {
		test_parser<pType,tMode>(fileInfoVec_[i],timings_[i],numThreads_);
	}
}
