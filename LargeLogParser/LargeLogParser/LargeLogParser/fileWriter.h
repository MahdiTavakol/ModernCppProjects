#pragma once
#include <string>
#include <filesystem>
#include <array>
#include <fstream>

using std::string;

class fileWriter
{
public:
	fileWriter(const std::string& fileName_,
		const int& numLines_,
		const double& infoPercent_,
		const double& warnPercent_,
		const double& errorPercent_,
		const int& lineLength_ = 256) :
		fileName{fileName_},
		numLines{ numLines_ }, infoPercent{ infoPercent_ },
		warnPercent{ warnPercent_ }, errorPercent{ errorPercent_ },
		lineLength{ lineLength_ } 
	{
		file.open(fileName_, std::ios::out);
	}

	fileWriter(const fileWriter& rhs_): 
	    fileName{rhs_.fileName + "_2"},
		numLines{rhs_.numLines}, lineLength{rhs_.lineLength},
		infoPercent{rhs_.infoPercent}, warnPercent{rhs_.warnPercent}, errorPercent{rhs_.errorPercent} {}

	/*  
	 * since std::unique_ptr<fileWriter> fileWriterPtr2 = 
	 *   std::make_unique<fileWriter>(*fileWriterPtr); causes object slicing I need this
	 */
	virtual std::unique_ptr<fileWriter> clone() const = 0; 

	void inputArgs(std::array<int,2>& lineArg, std::array<double, 3>& contntArg) const {
		lineArg = {numLines, lineLength};
		contntArg = {infoPercent, warnPercent, errorPercent};
	}

	int operator()(std::array<int,3>& nums_) const {
		nums_[0] = numInfos; nums_[1] = numWarns;  nums_[2] = numErrors;
		return numLines;
	}

	void setNums(const std::array<int,3> nums_)
	{
		numInfos = nums_[0]; numWarns = nums_[1]; numErrors = nums_[2];
	}

	virtual void writeLine(const std::string& line_) = 0;
	void close() {file.close();}
	void removeFile() {std::filesystem::remove(fileName);} 

protected:
    // our input 
	std::string fileName;
	int numLines, lineLength;
	double infoPercent, warnPercent, errorPercent;
	// what we will get 
	int numInfos, numWarns, numErrors;

	std::ofstream file;
};


class fileWriterSimple : public fileWriter {
public:
	fileWriterSimple(const std::string& fileName_,
		const int& numLines_,
		const double& infoPercent_,
		const double& warnPercent_,
		const double& errorPercent_,
		const int& lineLength_ = 256) :
		fileWriter{fileName_, numLines_, infoPercent_, warnPercent_, errorPercent_, lineLength_} {}

	fileWriterSimple(const fileWriterSimple& rhs_): fileWriter{rhs_} {}

	std::unique_ptr<fileWriter> clone() const override {
		return std::make_unique<fileWriterSimple>(*this);
	} 

	void writeLine(const std::string& line_) override
	{
		file << line_ << std::endl;
	}
};