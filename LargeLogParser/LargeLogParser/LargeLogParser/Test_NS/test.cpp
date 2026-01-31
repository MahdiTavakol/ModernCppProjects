

#include "catch_amalgamated.hpp"
#include "test.hpp"

#include <vector>
#include <array>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>

// default parameters
constexpr int line_length = 128;
constexpr int num_test_sizes = 10;
constexpr int num_errwarninfo_combinations = 10;
constexpr int size_step = 1000;
constexpr int num_threads = 4;
constexpr double file_size_0 = 600.0;
constexpr int num_lines_0 = 1000;
constexpr double info_0 = 20.9;
constexpr double warn_0 = 10.7;
constexpr double error_0 = 35.5;

// default fileInfos
std::unique_ptr<fileInfo> fileInfoPtr =
    std::make_unique<fileInfo>(
		             /* filename */    "testFile.dat",
                     /* numLines*/     num_lines_0, 
					 /* info */		   info_0,
					 /* warns */	   warn_0,
					 /* errors */	   error_0);

std::unique_ptr<fileInfo> fileInfoPtrZero = 
	std::make_unique<fileInfo>(
		             /* filename */    "emptyTestFile.dat",
                     /* numLines*/     0, 
					 /* info */		   0.0,
					 /* warns */	   0.0,
					 /* errors */	   0.0
					);

// fileInfoVec
std::vector<std::unique_ptr<fileInfo>> fileInfoVec;



std::array<double,3> timingDifferentParsers;
std::array<std::vector<double>, 3> timingDifferentCombinations;
std::array<std::vector<double>, 3> timingDifferentSizes;
std::array<std::vector<double>, 2> timingDifferentThreads;
const std::vector<int> numThreadsVec = { 1,2,4,6,8 };

TEST_CASE("Testing without file generation for different log parsers")
{
	std::string fileName("1stTest.dat");
	std::array<int, 4> expectedValues = { 61, 12 , 4 , 23 };
	double dummyTiming;
	test_parser<TestingMode::NO_TIMING>
		(fileName,"SERIAL",expectedValues, num_threads, dummyTiming);
	test_parser<TestingMode::NO_TIMING>
		(fileName, "THREADS", expectedValues, num_threads, dummyTiming);
	test_parser<TestingMode::NO_TIMING>
		(fileName, "FUTURE", expectedValues, num_threads, dummyTiming);
}

TEST_CASE("Testing empty files with different log parsers")
{
	double dummyTiming;
 	test_parser<TestingMode::NO_TIMING>
	     (fileInfoPtrZero,"SERIAL",dummyTiming, num_threads);
    test_parser<TestingMode::NO_TIMING>
	     (fileInfoPtrZero,"THREADS",dummyTiming, num_threads);
    test_parser<TestingMode::NO_TIMING>
	     (fileInfoPtrZero,"FUTURE",dummyTiming, num_threads);
}

TEST_CASE("Testing files with different log parsers")
{
	test_parser<TestingMode::TIMING>
	     (fileInfoPtr,"SERIAL",timingDifferentParsers[0], num_threads);
    test_parser<TestingMode::TIMING>
	     (fileInfoPtr,"THREADS",timingDifferentParsers[1], num_threads);
    test_parser<TestingMode::TIMING>
	     (fileInfoPtr,"FUTURE",timingDifferentParsers[2], num_threads);
}

TEST_CASE("Printing timing info different parsers")
{
	std::cout << std::endl << std::endl;
	std::cout << "Timing for different parsers" << std::endl;
	std::cout << std::left
		<< std::setw(20) << "Serial"
		<< std::setw(20) << "Parallel"
		<< std::setw(20) << "ParallelFuture"
		<< std::endl;

	std::cout << std::string(60, '-') << '\n';

	std::cout << std::left
		<< std::setw(20) << timingDifferentParsers[0]
		<< std::setw(20) << timingDifferentParsers[1]
		<< std::setw(20) << timingDifferentParsers[2]
		<< '\n';

	std::cout << std::endl << std::endl;
	SUCCEED("Timing results printed to console.");
}

TEST_CASE("Filling the fileInfoVec with multiple combinations")
{
	static std::random_device rd;
	static std::mt19937_64 rng{ rd() };
	static std::uniform_real_distribution<double> dist{ 0.0, 100.0};

	for (int i = 0; i < num_errwarninfo_combinations; i++)
	{
		double r1 = dist(rng);
		double r2 = dist(rng);
		double r3 = dist(rng);

		std::vector<double> vec = { r1,r2,r3 };
		std::sort(vec.begin(), vec.end());

		double infoPercent = vec[0];
		double warnPercent = vec[1] - vec[0];
		double errorPercent = vec[2] - vec[1];

		std::unique_ptr<fileInfo> fileInfoPtrI = 
		    std::make_unique<fileInfo>(
		             /* filename */    "testFile-" + std::to_string(i) + ".dat",
                     /* numLines*/     num_lines_0, 
					 /* info */		   infoPercent,
					 /* warns */	   warnPercent,
					 /* errors */	   errorPercent);

		fileInfoVec.push_back(std::move(fileInfoPtrI));
	}
	SUCCEED("The fileInfoVec was filled with mutiple combinations");
}

TEST_CASE("Testing the serial log parser with multiple file combinations")
{
	test_parser<TestingMode::TIMING>
	   (fileInfoVec, "SERIAL", timingDifferentCombinations[0], num_threads);
}

TEST_CASE("Testing the thread log parser with multiple file combinations")
{
	test_parser<TestingMode::TIMING>
	   (fileInfoVec, "THREADS",timingDifferentCombinations[1], num_threads);
}

TEST_CASE("Testing the future log parser with multiple file combinations")
{
	test_parser<TestingMode::TIMING>
	   (fileInfoVec, "FUTURE", timingDifferentCombinations[2], num_threads);
}

TEST_CASE("Printing timing info different file combinations with 4 threads")
{
	std::cout << std::endl << std::endl;
	std::cout << "Timing info for different file combinations with 4 threads" << std::endl;
	std::cout << std::left
		<< std::setw(18) << "File_Number"
		<< std::setw(20) << "Serial"
		<< std::setw(20) << "Threads"
		<< std::setw(20) << "Future"
		<< std::endl;

	std::cout << std::string(78, '-') << std::endl;

	for (int i = 0; i < num_errwarninfo_combinations; i++)
		std::cout << std::left
		<< std::setw(18) << i
		<< std::setw(20) << timingDifferentCombinations[0][i]
		<< std::setw(20) << timingDifferentCombinations[1][i]
		<< std::setw(20) << timingDifferentCombinations[2][i]
		<< std::endl;

	std::cout << std::endl << std::endl;
	SUCCEED("Timing results printed to console.");
}

TEST_CASE("Filling the fileInfoVec with multiple file sizes")
{
	fileInfoVec.clear();
	// I do not want to deallocate.. that is the reason why I did not use the erase method

	for (int i = 0; i < num_test_sizes; i++)
	{
		int num_lines = (i+1) * size_step + num_lines_0; 


		std::unique_ptr<fileInfo> fileInfoPtrI = 
		    std::make_unique<fileInfo>(
		             /* filename */   "testFile-" + std::to_string(i) + ".dat",
                     /* numLines*/    num_lines, 
					 /* info */	      info_0,
					 /* warns */	  warn_0,
					 /* errors */	  error_0);

		fileInfoVec.push_back(std::move(fileInfoPtrI));
	}
	SUCCEED("The fileInfoVec was filled with mutiple file sizes");
}

TEST_CASE("Testing the serial log parser with multiple file sizes")
{
	test_parser<TestingMode::TIMING>
	   (fileInfoVec,"SERIAL", timingDifferentSizes[0], num_threads);
}

TEST_CASE("Testing the threads log parser with multiple file sizes")
{
	test_parser<TestingMode::TIMING>
	   (fileInfoVec, "THREADS", timingDifferentSizes[1], num_threads);
}

TEST_CASE("Testing the future log parser with multiple file sizes")
{
	test_parser<TestingMode::TIMING>
	   (fileInfoVec, "FUTURE", timingDifferentSizes[2], num_threads);
}

TEST_CASE("Printing timing info different methods with 4 threads")
{
	std::cout << std::endl << std::endl;
	std::cout << "Timing for different file sizes (4 threads)" << std::endl;
	std::cout << std::left
		<< std::setw(18) << "File_size"
		<< std::setw(20) << "Serial"
		<< std::setw(20) << "Threads"
		<< std::setw(20) << "Future"
		<< std::endl;

	std::cout << std::string(78, '-') << std::endl;

	for (int i = 0; i < num_test_sizes; i++)
		std::cout << std::left
		<< std::setw(18) << static_cast<double>(i + 1) * size_step
		<< std::setw(20) << timingDifferentSizes[0][i]
		<< std::setw(20) << timingDifferentSizes[1][i]
		<< std::setw(20) << timingDifferentSizes[2][i]
		<< std::endl;
	SUCCEED("Timing results printed to console.");
}

TEST_CASE("Testing the threads log parser with multiple thread numbers")
{
	for (const auto& numThreads : numThreadsVec) 
		test_parser<TestingMode::TIMING>
			(fileInfoVec,"THREADS", timingDifferentThreads[0], numThreads);
}

TEST_CASE("Testing the parallel future log parser with multiple thread numbers")
{
	for (const auto& numThreads : numThreadsVec) 
		test_parser<TestingMode::TIMING>
			(fileInfoVec,"FUTURE",timingDifferentThreads[1], numThreads);
}

TEST_CASE("Printing timing info different methods with different threads")
{
	std::cout << std::endl << std::endl;
	std::cout << "Timing for different thread numbers" << std::endl;
	std::cout << std::left
		<< std::setw(18) << "Num_threads"
		<< std::setw(20) << "Threads"
		<< std::setw(20) << "Future"
		<< std::endl;

	std::cout << std::string(78,'-') << std::endl;

	for (int i = 0; i < numThreadsVec.size(); i++)
		std::cout << std::left
		<< std::setw(18) << numThreadsVec[i]
		<< std::setw(20) << timingDifferentThreads[0][i]
		<< std::setw(20) << timingDifferentThreads[1][i]
		<< std::endl;

	std::cout << std::endl << std::endl;
	SUCCEED("Timing results printed to console.");
}