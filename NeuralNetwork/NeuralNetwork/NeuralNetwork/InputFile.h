#pragma once

#include <Eigen/Dense>
#include <array>
#include <string>
#include <fstream>
#include "Logger.h"


using std::string;
using std::array;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::RowVectorXd;
using std::ifstream;

enum {NO_SHUFFLE = 0, SHUFFLE = 1 << 0};

class InputFile
{
public:
	InputFile(Logger& logger_, const int& numTargetCols_, const int& shuffleMode_ = NO_SHUFFLE, const int& shuffleSeed_ = 123);
	InputFile(Logger& logger_, const string& dataFileName_, const int& numTargetCols_,
		const int& shuffleMode_ = NO_SHUFFLE, const int& shuffleSeed_ = 123);
	void resetFileName(const string& dataFileName_);
	void init();
	virtual void read();
	void return_data(array<int, 2>& InputFileDim_, array<int, 2>& outputDim_, MatrixXd& InputFileMatrix_, MatrixXd& outputMatrix_);
	InputFile(const InputFile&) = delete;
	const InputFile& operator=(const InputFile&) = delete;
	InputFile(InputFile&&) = delete;
	InputFile& operator=(InputFile&&) = delete;
	virtual ~InputFile() = default;
protected:
	Logger& logger;
	int shuffleMode;
	int shuffleSeed;
	std::vector<int> shuffleIndex;
	ifstream dataFile;
	const int numTargetCols;
	array<int, 2> fileDim;
	array<int, 2> InputFileDim;
	array<int, 2> outputDim;
	MatrixXd InputFileMatrix;
	MatrixXd outputMatrix;
	array<int, 2> readCSVFileDim(ifstream& file_);
	void readCSVFile(ifstream& file_, MatrixXd& InputFileData_, MatrixXd& outputData_,
		array<int, 2>& InputFileDim_, array<int,2>& outputDim_) {
		if (InputFileDim_[0] != outputDim_[0])
			throw std::runtime_error("Internal Error!");
		array<int, 2> range{ 0, InputFileDim_[0]-1};
		readCSVFile(file_, InputFileData_, outputData_, InputFileDim_, outputDim_, range);
	};
	void readCSVFile(ifstream& file_, MatrixXd& InputFileData_, MatrixXd& outputData_, 
		array<int, 2>& InputFileDim_, array<int, 2> outputDim_, array<int, 2>& indxRange_);
	void returnDim(array<int, 2>& InputFileDim_, array<int, 2>& outputDim_)
	{
		InputFileDim_ = InputFileDim; outputDim_ = outputDim;
	}
};
