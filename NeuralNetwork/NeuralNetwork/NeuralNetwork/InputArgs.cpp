#include "InputArgs.h"

InputArgs::InputArgs(int argc, char** argv)
{
	if (argc == 1)
	{
		defaultArgs();
	}
	else if (argc <= 8)
		throw std::invalid_argument("Not enough input args");
	else {
		trainFileName = std::string(argv[1]);
		testFileName = std::string(argv[2]);
		logFileName = std::string(argv[3]);
		numMaxLayers = std::stoi(argv[4]);
		numTargetCols = std::stoi(argv[5]);
		batchSize = std::stoi(argv[6]);

		numLayers = std::stoi(argv[7]);

		if (argc < 8 + 3*numLayers)
			throw std::invalid_argument("Not enough input args");

		Layers.reserve(numLayers);

		int iarg = 8;
		while (iarg < argc) {
			layerData layerI;
			std::string type = std::string(argv[iarg]);
			int inputDim = std::stoi(argv[iarg + 1]);
			if (type == "Layer") {
				int outputDim = std::stoi(argv[iarg + 2]);
				layerI.layerType = LAYER;
				layerI.info.outputDim = outputDim;
				
			}
			else if (type == "Drop") {
				double dropRate = std::stod(argv[iarg + 2]);
				layerI.layerType = DROPLAYER;
				layerI.info.dropRate = dropRate;
			}
			layerI.intputDim = inputDim;
			Layers.push_back(layerI);
			iarg += 3;
		}
	}
}

void InputArgs::returnFileNames(std::string& trainFileName_, std::string& testFileName_, std::string& logFileName_) const
{
	trainFileName_ = trainFileName;
	testFileName_ = testFileName;
	logFileName_ = logFileName;
}

void InputArgs::returnNums(int& numMaxLayers_, int& numTargetCols_, int& batchSize_) const
{
	numMaxLayers_ = numMaxLayers;
	numTargetCols_ = numTargetCols;
	batchSize_ = batchSize;
}

vector<layerData> InputArgs::returnLayers()
{
	if (numLayers == 0)
	{
		throw std::runtime_error("The layers have been returned before!!!");
	}
	numLayers = 0;
	return std::move(Layers);
}

void InputArgs::defaultArgs()
{
	trainFileName = "data/train.csv";
	testFileName = "data/test.csv";
	logFileName = "log.txt";
	numMaxLayers = 13;
	numTargetCols = 1;
	batchSize = 32;
	numLayers = 15;

	Layers.reserve(numLayers);

	layerData dataI;


	dataI.layerType = LAYER;
	dataI.intputDim = 100;
	dataI.info.outputDim = 90;
	Layers.push_back(dataI);
	dataI.layerType = DROPLAYER;
	dataI.intputDim = 90;
	dataI.info.dropRate = 0.2;
	Layers.push_back(dataI);
	dataI.layerType = LAYER;
	dataI.intputDim = 90;
	dataI.info.outputDim = 80;
	Layers.push_back(dataI);
	dataI.layerType = DROPLAYER;
	dataI.intputDim = 80;
	dataI.info.dropRate = 0.2;
	Layers.push_back(dataI);
	dataI.layerType = LAYER;
	dataI.intputDim = 80;
	dataI.info.outputDim = 70;
	Layers.push_back(dataI);
	dataI.layerType = DROPLAYER;
	dataI.intputDim = 70;
	dataI.info.dropRate = 0.2;
	Layers.push_back(dataI);
	dataI.layerType = LAYER;
	dataI.intputDim = 70;
	dataI.info.outputDim = 50;
	Layers.push_back(dataI);
	dataI.layerType = DROPLAYER;
	dataI.intputDim = 50;
	dataI.info.dropRate = 0.2;
	Layers.push_back(dataI);
	dataI.layerType = LAYER;
	dataI.intputDim = 50;
	dataI.info.outputDim = 30;
	Layers.push_back(dataI);
	dataI.layerType = DROPLAYER;
	dataI.intputDim = 30;
	dataI.info.dropRate = 0.2;
	Layers.push_back(dataI);
	dataI.layerType = LAYER;
	dataI.intputDim = 30;
	dataI.info.outputDim = 10;
	Layers.push_back(dataI);
	dataI.layerType = DROPLAYER;
	dataI.intputDim = 10;
	dataI.info.dropRate = 0.2;
	Layers.push_back(dataI);
	dataI.layerType = LAYER;
	dataI.intputDim = 10;
	dataI.info.outputDim = 2;
	Layers.push_back(dataI);
	dataI.layerType = DROPLAYER;
	dataI.intputDim = 2;
	dataI.info.dropRate = 0.2;
	Layers.push_back(dataI);
	dataI.layerType = LAYER;
	dataI.intputDim = 2;
	dataI.info.outputDim = 1;
	Layers.push_back(dataI);
}
