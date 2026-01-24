#pragma once

enum class TestingMode
{
	TIMING_INFO,
	NO_TIMING_INFO
};

enum class ParserType
{
	SERIAL,
	PARALLEL,
	PARALLELFUTURE
};



void CreateLogs(const std::string& log_file_name_, const fileProp& file_prop_,  std::array<int, 4>& expectedValues_)
{
	CreateLargeLogs logCreator{ log_file_name_,file_prop_.numLines,file_prop_.lineLength,
		file_prop_.infoPercent,file_prop_.warnPercent,file_prop_.errorPercent};
	logCreator.generateLog();
	expectedValues_[0] = logCreator(expectedValues_[1], expectedValues_[2], expectedValues_[3]);
}

template<typename Func, TestingMode tMode>
double run_parser(Func&& f)
{
	if constexpr (tMode == TestingMode::TIMING_INFO)
	{
		auto t1 = high_resolution_clock::now();
		f();
		auto t2 = high_resolution_clock::now();
		duration<double, std::milli> ms_double = t2 - t1;
		double duration = ms_double.count();
		return duration;
	}
	else f();
	return 0.0;
}

template<ParserType pType, TestingMode tMode>
void test_parser(std::string& logFileName, const std::array<int, 4>& expectedValues,
	const int& numThreads, double& timingInfo)
{
	std::array<int, 4> outputValues;

	if constexpr (pType == ParserType::SERIAL) {
		// Serial log parser
		LogParser logParser{ logFileName };
		// Since the logParserParallelRunner invokes the initialize of the LogParserParallel!
		auto func = [&]()
			{
				logParser.initialize();
				logParser.readFile();
			};
		timingInfo = run_parser<tMode>(func);
		outputValues[0] = logParser(outputValues[1], outputValues[2], outputValues[3]);
	}
	else if constexpr (pType == ParserType::PARALLEL) {
		// Parallel log parser
		LogParserParallelRunner logParserParallelRunner{ logFileName,numThreads };
		auto func = [&]()
			{
				logParserParallelRunner.parseLogs();
			};
		timingInfo = run_parser<tMode>(func);
		outputValues[0] = logParserParallelRunner(outputValues[1], outputValues[2], outputValues[3]);
	}
	else if constexpr (pType == ParserType::PARALLELFUTURE) {
		// Parallel log parser with future
		LogParserParallelFuture logParserParallelFuture{ logFileName, numThreads };
		auto func = [&]()
			{
				logParserParallelFuture.initialize();
				logParserParallelFuture.readFile();
			};
		timingInfo = run_parser<tMode>(func);
		outputValues[0] = logParserParallelFuture(outputValues[1], outputValues[2], outputValues[3]);
	}
	else
		throw std::invalid_argument("Wrong argument");


	for (int j = 0; j < 4; j++)
		REQUIRE(expectedValues[j] == outputValues[j]);
}

template<TestingMode tMode>
void test_multiple_parsers(const int& num_threads_, const fileProp& fileProp_, std::array<double,3>& timings_)
{
	std::array<int, 4> expectedValues;
	// These dont matter!
	double infoPercent = 10.0, warnPercent = 20.0, errorPercent = 35.0;
	int numLines = fileProp_.numLines;
	std::string logFileName = "test-" + std::to_string(numLines) + "-lines.dat";
	CreateLogs(logFileName, fileProp_, expectedValues);

	test_parser<ParserType::SERIAL, tMode>(logFileName, expectedValues, num_threads, timings_[0]);
	test_parser<ParserType::PARALLEL, tMode>(logFileName, expectedValues, num_threads, timings_[1]);
	test_parser<ParserType::PARALLELFUTURE, tMode>(logFileName, expectedValues, num_threads, timings_[2]);
}

template<ParserType pType, TestingMode tMode>
void test_multiple_file_combinations(std::vector<fileProp>& filePropVec_, std::vector<double>& timings_,
	const int& numThreads)
{
	std::array<int, 4> expectedValues;
	timings_.resize(filePropVec_.size());
	for (int i = 0; i < numTests_; i++)
	{
		double fileSize = static_cast<double>(i + 1) * size_step_;
		int numLines = (fileSize + filePropVec_[i].lineLength - 1) / filePropVec_[i].lineLength;
		myFileProp.numLines = numLines;
		std::string logFileName = "log-" + std::to_string(i + 1) + ".dat";
		CreateLogs(logFileName, filePropVec_[i], expectedValues);
		double& duration = timings_[i];
		test_parser<pType, tMode>(logFileName, expectedValues, numThreads_, duration);
	}
}

template<ParserType pType, TestingMode tMode>
void test_multiple_file_sizes(const int& numTests_, std::vector<double>& timings_, 
	const double& size_step_, const int& numThreads_, const fileProp& fileProp_)
{
	std::array<int, 4> expectedValues;
	timings_.resize(numTests_);
	fileProp myFileProp = fileProp_;
	for (int i = 0; i < numTests_; i++)
	{
		double fileSize = static_cast<double>(i + 1) * size_step_;
		int numLines = (fileSize + fileProp_.lineLength - 1) / fileProp_.lineLength;
		myFileProp.numLines = numLines;
		std::string logFileName = "log-" + std::to_string(i + 1) + ".dat";
		CreateLogs(logFileName, myFileProp,  expectedValues);
		double& duration = timings_[i];
		test_parser<pType, tMode>(logFileName, expectedValues, numThreads_, duration);
	}
}

template<ParserType pType, TestingMode tMode>
void test_multiple_thread_nums(const std::vector<int>& numThreadsVec_, std::vector<double>& timings_, const fileProp fileProp_)
{
	std::array<int, 4> expectedValues;
	timings_.resize(numThreadsVec_.size());
	int numLines = (file_size_0 + fileProp_.lineLength - 1) / fileProp_.lineLength;
	std::string logFileName = "log-size-0.dat";
	fileProp myFileProp = fileProp_;
	fileProp_.numLines = numLines;
	CreateLogs(logFileName, myFileProp,  expectedValues);
	for (int i = 0; i < numThreadsVec_.size(); i++)
	{
		int thread_num = numThreadsVec_[i];
		double& duration = timing_[i];
		test_parser<pType, tMode>(logFileName, expectedValues, thread_num, duration);
	}
}