#include "CreateLargeLogs.h"
#include "LogParser.h"
#include "LogParserFuture.h"
#include "LogParserThreads.h"

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

// default fileWriters
std::unique_ptr<fileWriter> fileWriterPtr =
    std::make_unique<fileWriterSimple>(
		             /* filename */    "testFile.dat",
                     /* numLines*/     num_lines_0, 
					 /* info */		   info_0,
					 /* warns */	   warn_0,
					 /* errors */	   error_0);

std::unique_ptr<fileWriter> fileWriterPtrZero = 
	std::make_unique<fileWriterSimple>(
		             /* filename */    "emptyTestFile.dat",
                     /* numLines*/     0, 
					 /* info */		   0.0,
					 /* warns */	   0.0,
					 /* errors */	   0.0
					);

// fileWriterVec
std::vector<std::unique_ptr<fileWriter>> fileWriterVec;



std::array<double,3> timingDifferentParsers;
std::array<std::vector<double>, 3> timingDifferentCombinations;
std::array<std::vector<double>, 3> timingDifferentSizes;
std::array<std::vector<double>, 2> timingDifferentThreads;
const std::vector<int> numThreadsVec = { 1,2,4,6,8 };


TEST_CASE("Testing empty files with different log parsers")
{
	double dummyTiming;
	
	test_parser<ParserType::SERIAL,TestingMode::NO_TIMING>
	     (fileWriterPtrZero,dummyTiming, num_threads);
    test_parser<ParserType::THREADS,TestingMode::NO_TIMING>
	     (fileWriterPtrZero,dummyTiming, num_threads);
    test_parser<ParserType::FUTURE,TestingMode::NO_TIMING>
	     (fileWriterPtrZero,dummyTiming, num_threads);
}

TEST_CASE("Testing files with different log parsers")
{
	test_parser<ParserType::SERIAL,TestingMode::NO_TIMING>
	     (fileWriterPtr,timingDifferentParsers[0], num_threads);
    test_parser<ParserType::THREADS,TestingMode::NO_TIMING>
	     (fileWriterPtr,timingDifferentParsers[1], num_threads);
    test_parser<ParserType::FUTURE,TestingMode::NO_TIMING>
	     (fileWriterPtr,timingDifferentParsers[2], num_threads);
}

TEST_CASE("Printing timing info different parsers")
{
	std::cout << "Serial ------------ Parallel ------------- ParallelFuture" << std::endl;
	std::cout 
		<< "-------------"
		<< timingDifferentParsers[0]
		<< "-------------"
		<< timingDifferentParsers[1]
		<< "-------------"
		<< timingDifferentParsers[2]
		<< std::endl;

	SUCCEED("Timing results printed to console.");
}

TEST_CASE("Filling the fileWriterVec with multiple combinations")
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

		std::unique_ptr<fileWriter> fileWriterPtrI = 
		    std::make_unique<fileWriterSimple>(
		             /* filename */    "testFile-" + std::to_string(i) + ".dat",
                     /* numLines*/     num_lines_0, 
					 /* info */		   infoPercent,
					 /* warns */	   warnPercent,
					 /* errors */	   errorPercent);

		fileWriterVec.push_back(fileWriterPtrI);
	}
	SUCCEED("The fileWriterVec was filled with mutiple combinations");
}

TEST_CASE("Testing the serial log parser with multiple file combinations")
{
	test_parser<ParserType::SERIAL, TestingMode::TIMING>
	   (fileWriterVec, timingDifferentCombinations[0], num_threads);
}

TEST_CASE("Testing the thread log parser with multiple file combinations")
{
	test_parser<ParserType::THREADS, TestingMode::TIMING>
	   (fileWriterVec, timingDifferentCombinations[1], num_threads);
}

TEST_CASE("Testing the future log parser with multiple file combinations")
{
	test_parser<ParserType::SERIAL, TestingMode::TIMING>
	   (fileWriterVec, timingDifferentCombinations[2], num_threads);
}

TEST_CASE("Printing timing info different file combinations with 4 threads")
{
	std::cout << std::left
		<< std::setw(18) << "File_combination"
		<< std::setw(20) << "Serial"
		<< std::setw(20) << "Threads"
		<< std::setw(20) << "Future"
		<< std::endl;

	for (int i = 0; i < num_errwarninfo_combinations; i++)
		std::cout << std::left
		<< std::setw(18) << i
		<< std::setw(20) << timingDifferentCombinations[0][i]
		<< std::setw(20) << timingDifferentCombinations[1][i]
		<< std::setw(20) << timingDifferentCombinations[2][i]
		<< std::endl;
	SUCCEED("Timing results printed to console.");
}

TEST_CASE("Filling the fileWriterVec with multiple file sizes")
{
	fileWriterVec.clear();
	// I do not want to deallocate.. that is the reason why I did not use the erase method

	for (int i = 0; i < num_test_sizes; i++)
	{
		int num_lines = (i+1) * size_step + num_lines_0; 


		std::unique_ptr<fileWriter> fileWriterPtrI = 
		    std::make_unique<fileWriterSimple>(
		             /* filename */    "testFile-" + std::to_string(i) + ".dat",
                     /* numLines*/     num_lines, 
					 /* info */		   info_0,
					 /* warns */	   warn_0,
					 /* errors */	   error_0);

		fileWriterVec.push_back(fileWriterPtrI);
	}
	SUCCEED("The fileWriterVec was filled with mutiple file sizes");
}

TEST_CASE("Testing the serial log parser with multiple file sizes")
{
	test_parser<ParserType::SERIAL, TestingMode::TIMING>
	   (fileWriterVec, timingDifferentSizes[0], num_threads);
}

TEST_CASE("Testing the threads log parser with multiple file sizes")
{
	test_parser<ParserType::THREADS, TestingMode::TIMING>
	   (fileWriterVec, timingDifferentSizes[1], num_threads);
}

TEST_CASE("Testing the future log parser with multiple file sizes")
{
	test_parser<ParserType::FUTURE, TestingMode::TIMING>
	   (fileWriterVec, timingDifferentSizes[2], num_threads);
}

TEST_CASE("Printing timing info different methods with 4 threads")
{
	std::cout << std::left
		<< std::setw(18) << "File_size"
		<< std::setw(20) << "Serial"
		<< std::setw(20) << "Parallel"
		<< std::setw(20) << "ParallelFuture"
		<< std::endl;

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
		test_parser<ParserType::THREADS,TestingMode::TIMING>
			(fileWriterVec,timingDifferentThreads[0],numThreads);
}

TEST_CASE("Testing the parallel future log parser with multiple thread numbers")
{
	for (const auto& numThreads : numThreadsVec) 
		test_parser<ParserType::FUTURE,TestingMode::TIMING>
			(fileWriterVec,timingDifferentThreads[1],numThreads);
}

TEST_CASE("Printing timing info different methods with different threads")
{
	std::cout << std::left
		<< std::setw(18) << "Num_threads"
		<< std::setw(20) << "Threads"
		<< std::setw(20) << "Future"
		<< std::endl;

	for (int i = 0; i < num_test_sizes; i++)
		std::cout << std::left
		<< std::setw(18) << numThreadsVec[i]
		<< std::setw(20) << timingDifferentThreads[0][i]
		<< std::setw(20) << timingDifferentThreads[1][i]
		<< std::endl;
	SUCCEED("Timing results printed to console.");
}