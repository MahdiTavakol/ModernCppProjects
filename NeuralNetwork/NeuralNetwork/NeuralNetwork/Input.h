#pragma once

#include <Eigen/Dense>
#include <array>
#include <string>
#include <fstream>

using std::string;
using std::array;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::ifstream;

class Input
{
public:
	Input(const string& inputFileName_, const string& outputFileName_);
	void read();
	void return_data(array<int, 2> inputDim_, array<int, 2> outputDim_, MatrixXd& inputMatrix_, MatrixXd& outputMatrix_);
	
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(Input&&) = delete;
	~Input() = default;

private:
	ifstream inputFile, outputFile;
	array<int, 2> inputDim;
	array<int, 2> outputDim;
	MatrixXd inputMatrix;
	MatrixXd outputMatrix;

	array<int, 2> readCSVFile(ifstream& file_, MatrixXd& output_);

	void returnDim(array<int, 2>& inputDim_, array<int, 2>& outputDim_)
	{
		inputDim_ = inputDim; outputDim_ = outputDim;
	}
};
