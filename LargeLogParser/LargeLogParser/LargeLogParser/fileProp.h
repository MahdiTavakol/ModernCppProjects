#pragma once

class fileProp
{
public:
	fileProp(const std::string& fileName_,
		const int& numLines_,
		const double& infoPercent_,
		const double& warnPercent_,
		const double& errorPercent_,
		const int& lineLength_ = 256) :
		numLines{ numLines_ }, infoPercent{ infoPercent_ },
		warnPercent{ warnPercent_ }, errorPercent{ errorPercent_ },
		lineLength{ lineLength_ } 
	{
		file.open(fileName_, std::ios::out);
	}

	int operator()(int& num_infos_, int& num_warns_, int& num_errors_) const {
		num_infos_ = numInfos; 
		num_warns_ = numWarns; 
		num_errors_ = numErrors;
	}

	virtual void writeLine(const std::string& line_) = 0;

private:
	int numLines, lineLength;
	double infoPercent, warnPercent, errorPercent;
	int numInfos, numWarns, numErrors;

	std::ofstream file;
};


class filePropSimple : public fileProp {
public:
	filePropSimple(const std::string& fileName_,
		const int& numLines_,
		const double& infoPercent_,
		const double& warnPercent_,
		const double& errorPercent_,
		const int& lineLength_ = 256) :
		fileProp{fileName_, numLines_, infoPercent_, warnPercent_, errorPercent_, lineLength_} {}

	void writeLine(const std::string& line_) override
	{
		file << line_ << std::endl;
	}
};