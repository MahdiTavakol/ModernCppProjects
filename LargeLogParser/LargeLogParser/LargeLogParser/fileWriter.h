#pragma once
#include <string>
#include <filesystem>
#include <array>
#include <fstream>
#include <memory>

#include <iostream>

using std::string;

// fileInfo has information on the 
// file size, file name and the precentage of
// each of the contents in the file

// while the fileWriter has 
// the fileInfo plus the number of each 
// of error, warn and info lines.
// The reason for such a design is 
// that having stream inside a class
// makes its copying troublesome.
// So here we separated a writer class
// which cannot be copied and it contains 
// the copyable fileinfo class.


class fileInfo
{
public:
	friend class fileWriter;
	fileInfo(const std::string& fileName_,
			const int& numLines_,
			const double& infoPercent_,
			const double& warnPercent_,
			const double& errorPercent_,
			const int& lineLength_ = 256) :
			fileName{ fileName_ },
			numLines{ numLines_ }, infoPercent{ infoPercent_ },
			warnPercent{ warnPercent_ }, errorPercent{ errorPercent_ },
			lineLength{ lineLength_ }
	{}

	fileInfo(const fileInfo& rhs_) = default;

	std::string returnFileName() const { return fileName; }

protected:
	// our input 
	std::string fileName;
	int numLines, lineLength;
	double infoPercent, warnPercent, errorPercent;
};

class fileWriter
{
public:
	fileWriter(const std::unique_ptr<fileInfo>& info_) 
	{
		info = std::make_unique<fileInfo>(*info_);
		file.open(info->fileName);
	}

	// I do not want to reopen the stream!
	fileWriter(const fileWriter& rhs_) = delete;

	/* C++ itself takes care of closing the file when an object of
	   this class goes out of scope.. I have just put this here to 
	   emphasize 
	*/

	~fileWriter() {
		file.close();
	}

	void inputArgs(std::array<int,2>& lineArg, std::array<double, 3>& contntArg) const {
		lineArg = {info->numLines, info->lineLength};
		contntArg = {info->infoPercent, info->warnPercent, info->errorPercent};
	}

	int operator()(std::array<int,3>& nums_) const {
		nums_[0] = numInfos; nums_[1] = numWarns;  nums_[2] = numErrors;
		return info->numLines;
	}

	void setNums(const std::array<int,3> nums_)
	{
		numInfos = nums_[0]; numWarns = nums_[1]; numErrors = nums_[2];
	}


	virtual void writeLine(const std::string& line_) = 0;
	void close() {file.close();}
	void removeFile() {std::filesystem::remove(info->fileName.c_str());} 
	void openFile() { file.open(info->fileName); }

protected:
    // our input 
	std::unique_ptr<fileInfo> info;
	// what we will get 
	int numInfos{0}, numWarns{0}, numErrors{0};
	// the stream
	std::ofstream file;
};


class fileWriterSimple : public fileWriter {
public:
	fileWriterSimple(const std::unique_ptr<fileInfo>& info_) :
		fileWriter{info_} {}

	fileWriterSimple(const fileWriterSimple& rhs_) = delete;

	void writeLine(const std::string& line_) override
	{
		file << line_ << std::endl;
	}
};