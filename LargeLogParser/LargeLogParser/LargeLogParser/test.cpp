#include "CreateLargeLogs.h"
#include "LogParser.h"
#include "LogParserParallel.h"
#include "LogParserParallelFuture.h"
#include "LogParserParallelRunner.h"

#include "catch_amalgamated.hpp"
#include "test.hpp"

#include <vector>
#include <array>
#include <chrono>
#include <iostream>
#include <iomanip>

constexpr int line_length = 128;
constexpr int num_test_sizes = 10;
constexpr int num_errwarninfo_combinations = 10;
constexpr int size_step = 100;
constexpr int num_threads = 4;
constexpr double file_size_0 = 600.0;

fileProp file_prop = {1000,line_length, 20.9,10.7,35.5};
std::vector<fileProp> filePropVec;

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

std::array<double,3> timingDifferentParsers;
std::array<std::vector<double>, 3> timingDifferentCombinations;
std::array<std::vector<double>, 3> timingDifferentSizes;
std::array<std::vector<double>, 2> timingDifferentThreads;
const std::vector<int> numThreadsVec = { 1,2,4,6,8 };


TEST_CASE("Testing empty files with different log parsers")
{
	std::array<double, 3> dummyTimings;
	fileProp myFileProp = file_prop;
	myFileProp.numLines = 0;
	test_multiple_parsers<TestingMode::NO_TIMING_INFO>(num_threads, file_prop, dummyTimings);
}

TEST_CASE("Testing files with different log parsers")
{
	std::array<double, 3> dummyTimings;
	test_multiple_parsers<TestingMode::NO_TIMING_INFO>(num_threads, file_prop, timingDifferentParsers);
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

TEST_CASE("Filling the file_prop vector")
{
	static std::random_device rd;
	static std::mt19937_64 rng{ rd() };
	static std::uniform_real_distribution<> dist{ 0, 80 };

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

		fileProp myfileProp = file_prop;
		myfileProp.infoPercent = infoPercent;
		myfileProp.warnPercent = warnPercent;
		myfileProp.errorPercent = errorPercent;

		filePropVec.push_back(myfileProp);
	}
	SUCCEED("The file_prop vec was filled.");
}

TEST_CASE("Testing the serial log parser with multiple file combinations")
{
	test_multiple_file_combinations<ParserType::SERIAL, TestingMode::TIMING_INFO>(filePropVec, timingDifferentCombinations[0],
		num_threads);
}

TEST_CASE("Testing the parallel log parser with multiple file combinations")
{
	test_multiple_file_combinations<ParserType::PARALLEL, TestingMode::TIMING_INFO>(filePropVec, timingDifferentCombinations[1],
		num_threads);
}

TEST_CASE("Testing the parallel future log parser with multiple file combinations")
{
	test_multiple_file_combinations<ParserType::PARALLELFUTURE, TestingMode::TIMING_INFO>(filePropVec, timingDifferentCombinations[2],
		num_threads);
}

TEST_CASE("Printing timing info different file combinations with 4 threads")
{
	std::cout << std::left
		<< std::setw(18) << "File_combination"
		<< std::setw(20) << "Serial"
		<< std::setw(20) << "Parallel"
		<< std::setw(20) << "ParallelFuture"
		<< std::endl;

	for (int i = 0; i < num_test_sizes; i++)
		std::cout << std::left
		<< std::setw(18) << i
		<< std::setw(20) << timingDifferentCombinations[0][i]
		<< std::setw(20) << timingDifferentCombinations[1][i]
		<< std::setw(20) << timingDifferentCombinations[2][i]
		<< std::endl;
	SUCCEED("Timing results printed to console.");
}

TEST_CASE("Testing the serial log parser with multiple file sizes")
{
	test_multiple_file_sizes<ParserType::SERIAL, TestingMode::TIMING_INFO>(num_test_sizes,timingDifferentSizes[0],
		size_step,num_threads, file_prop);
}

TEST_CASE("Testing the parallel log parser with multiple file sizes")
{
	test_multiple_file_sizes<ParserType::PARALLEL, TestingMode::TIMING_INFO>(num_test_sizes,timingDifferentSizes[1], 
		size_step,num_threads,file_prop);
}

TEST_CASE("Testing the parallel future log parser with multiple file sizes")
{
	test_multiple_file_sizes<ParserType::PARALLELFUTURE, TestingMode::TIMING_INFO>(num_test_sizes,timingDifferentSizes[2], 
		size_step,num_threads,file_prop);
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

TEST_CASE("Testing the parallel log parser with multiple thread numbers")
{
	test_multiple_thread_nums<ParserType::PARALLEL, TestingMode::TIMING_INFO>(numThreadsVec, timingDifferentThreads[0],file_prop);
}

TEST_CASE("Testing the parallel future log parser with multiple thread numbers")
{
	test_multiple_thread_nums<ParserType::PARALLELFUTURE, TestingMode::TIMING_INFO>(numThreadsVec, timingDifferentThreads[1], file_prop);
}

TEST_CASE("Printing timing info different methods with different threads")
{
	std::cout << std::left
		<< std::setw(18) << "Num_threads"
		<< std::setw(20) << "Parallel"
		<< std::setw(20) << "ParallelFuture"
		<< std::endl;

	for (int i = 0; i < num_test_sizes; i++)
		std::cout << std::left
		<< std::setw(18) << numThreadsVec[i]
		<< std::setw(20) << timingDifferentThreads[0][i]
		<< std::setw(20) << timingDifferentThreads[1][i]
		<< std::endl;
	SUCCEED("Timing results printed to console.");
}




