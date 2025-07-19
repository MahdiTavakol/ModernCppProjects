#pragma once

#include <Eigen/Dense>
#include <array>
#include <string>
#include <fstream>

using std::string;
using std::array;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::RowVectorXd;
using std::ifstream;

class Input
{
public:
	Input(const int& numTargetCols_);
	Input(const string& dataFileName_, const int& numTargetCols_);
	void resetFileName(const string& dataFileName_);
	void init();
	virtual void read();
	void return_data(array<int, 2> inputDim_, array<int, 2> outputDim_, MatrixXd& inputMatrix_, MatrixXd& outputMatrix_);
	
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(Input&&) = delete;
	~Input() = default;

protected:
	ifstream dataFile;
	const int numTargetCols;
	array<int, 2> fileDim;
	array<int, 2> inputDim;
	array<int, 2> outputDim;
	MatrixXd inputMatrix;
	MatrixXd outputMatrix;

	array<int, 2> readCSVFileDim(ifstream& file_);
	void readCSVFile(ifstream& file_, MatrixXd& inputData_, MatrixXd& outputData_,
		array<int, 2>& inputDim_, array<int,2>& outputDim_) {
		if (inputDim_[0] != outputDim_[0])
			throw std::runtime_error("Internal Error!");
		array<int, 2> range{ 0, inputDim_[0]-1};
		readCSVFile(file_, inputData_, outputData_, inputDim_, outputDim_, range);
	};
	void readCSVFile(ifstream& file_, MatrixXd& inputData_, MatrixXd& outputData_, 
		array<int, 2>& inputDim_, array<int, 2> outputDim_, array<int, 2>& indxRange_);

	void returnDim(array<int, 2>& inputDim_, array<int, 2>& outputDim_)
	{
		inputDim_ = inputDim; outputDim_ = outputDim;
	}
};
