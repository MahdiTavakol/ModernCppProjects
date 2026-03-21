#pragma once

#include <string>
#include <array>
#include <vector>
#include <stdexcept>


using std::array;
using std::vector;

enum { LAYER= 0, DROPLAYER=1 };

union layerUnion
{
	int outputDim;
	double dropRate;
};

struct layerData {
	int layerType;
	int intputDim;
	layerUnion info;
};

class InputArgs
{
public:
	InputArgs(int argc, char** argv);
	void returnFileNames(std::string& trainFileName_, std::string& testFileName_, std::string& logFileName_) const;
	void returnNums(int& numMaxLayers_, int& numTargetCols_, int& batchSize_) const;
	vector<layerData> returnLayers();

private:


	std::string trainFileName;
	std::string testFileName;
	std::string logFileName;
	
	int numMaxLayers;
	int numTargetCols;
	int batchSize;

	int numLayers;
	vector<layerData> Layers;

	//just for testing
	void defaultArgs();

};
