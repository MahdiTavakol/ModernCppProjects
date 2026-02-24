#include "FixPrint.h"
#include "Particles.h"
#include "Error.h"

#include <iostream>

FixPrint::FixPrint(Engine& engine_, std::vector<std::string> args_):
	Fix{ engine_, args_ }
{
	// Fix print id nevery mask variable particleId
	// fix print 1 1 init_integrate x 0
	auto nargs = args_.size();
	if (nargs < 7) {
		auto& error = engine().getError();
		error->one("Not enough arguments for fix print command! Expected at least 7, got " + std::to_string(nargs));
		return;
	}
	variable = args_[5];
	particleId = std::stoi(args_[6]);
}

FixPrint::FixPrint(Engine& engine_,
	std::string id_,
	int nevery_ ,
	int mask_,
	std::string variable_,
	int particleId_) :
	Fix{ engine_,id_,nevery_ },
	variable{ variable_ },
	particleId{ particleId_ }
{
	mask = mask_;
}

void FixPrint::init()
{
	auto& particle = engine().getParticles();
	if (variable == "x") {
		inData = particle->getXData();
	}
	else if (variable == "v") {
		inData = particle->getVData();
	}
	else if (variable == "f") {
		inData = particle->getFData();
	}

}

void FixPrint::initial_integrate() {
	if (mask & INIT_INTEGRATE) 
		fillData();
}

void FixPrint::pre_force() {
	if (mask & PRE_FORCE)
		fillData();
}

void FixPrint::post_force() {
	if (mask & POST_FORCE)
		fillData();
}

void FixPrint::final_integrate() {
	if (mask & FINAL_INTEGRATE)
		fillData();
}

void FixPrint::fillData()
{
	std::array<double, 3> out =
	{ inData[3 * particleId], inData[3 * particleId + 1] , inData[3 * particleId + 2] };

	data.push_back(out);
}

const std::vector<std::array<double, 3>>& FixPrint::getOutputVector() const {
	return data;
}