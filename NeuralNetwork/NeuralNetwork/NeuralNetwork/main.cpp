#include "Activations.h"
#include "InputArgs.h"
#include "InputFile.h"
#include "InputMPIFile.h"
#include "LastLayerBatchEfficient.h"
#include "LayerBatchEfficient.h"
#include "Loss.h"
#include "Logger.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkMPI.h"
#include "NeuralNetworkOpenMP.h"
#include "Optimizers.h"
#include "Logger.h"

#include <memory>
#include <string>
#include <iostream>

using std::ostream;


enum class RunType
{
	SERIAL,
	MPI,
	OPENMP
};


int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	InputArgs args(argc, argv);
	vector<layerData> Layers;
	std::string trainFileName, testFileName, logFileName;
	int nMaxLayers, nTargetCols, bSize;

	Layers = args.returnLayers();
	args.returnFileNames(trainFileName, testFileName, logFileName);
	args.returnNums(nMaxLayers, nTargetCols, bSize);

	std::ofstream logFile{logFileName};
	std::vector<std::reference_wrapper<ostream>> logger_ref =
	{
		std::ref(std::cout),
		std::ref(logFile)
	};
	Logger logger{ logger_ref };


	RunType runType = RunType::MPI;
	std::unique_ptr<NeuralNetwork> neuralNetworkPtr;

	if (logger.log_level >= LOG_LEVEL_INFO)
	{
		logger << "Initializing the program" << std::endl;
		logger << "Creating the Neural Network" << std::endl;
	}

	switch (runType)
	{
	case RunType::SERIAL:
		// trainfiles, n_in (InputFile_cols) , n_out
		neuralNetworkPtr = std::make_unique<NeuralNetwork>(logger, trainFileName, testFileName, nTargetCols, nMaxLayers,  bSize);
		break;
	case RunType::MPI:
		neuralNetworkPtr = std::make_unique<NeuralNetworkMPI>(logger, trainFileName, testFileName, nTargetCols, nMaxLayers, bSize);
		break;
	case RunType::OPENMP:
		neuralNetworkPtr = std::make_unique<NeuralNetworkOpenMP>(logger, trainFileName, testFileName, nTargetCols, nMaxLayers, bSize);
		break;
	default:
		throw std::invalid_argument("Uknown run type");
	}

	neuralNetworkPtr->initializeInputFilePtr();
	neuralNetworkPtr->initializeOutputs();
	neuralNetworkPtr->readInputFileData();
	for (auto& layerI : Layers) {
		int inDim = layerI.intputDim;
		switch (layerI.layerType) {
		case LAYER:
			int ouDim = layerI.info.outputDim;
			neuralNetworkPtr->addLayer(inDim, ouDim);
			break;
		case DROPLAYER:
			double dropRate = layerI.info.dropRate;
			neuralNetworkPtr->addDropout(inDim, dropRate);
		case LASTLAYER:
			int ouDim = layerI.info.outputDim;
			neuralNetworkPtr->addLastLayer(inDim, ouDim);
			break;
		default:
			throw std::runtime_error("Unknown layertype!");
		}
	}
	neuralNetworkPtr->initializeLayers();
	neuralNetworkPtr->fit();
	neuralNetworkPtr->transform();


	MPI_Finalize();

	return 0;
}

