#include "FixPrint.h"
#include "Particles.h"
#include "Error.h"
#include <stdexcept>

#include <iostream>

FixPrint::FixPrint( std::vector<std::string> args_):
	Fix{args_ }
{
	// Fix print id nevery mask variable particleId
	// fix print 1 1 init_integrate x 0
	auto nargs = args_.size();
	if (nargs < 7) {
		throw std::invalid_argument("Not enough arguments for fix print command! Expected at least 7, got " + std::to_string(nargs));
		return;
	}
	variable = args_[5];
	particleId = std::stoi(args_[6]);
}

FixPrint::FixPrint(
	std::string id_,
	int nevery_ ,
	int mask_,
	std::string variable_,
	int particleId_) :
	Fix{id_,nevery_ },
	variable{ variable_ },
	particleId{ particleId_ }
{
	mask = mask_;
}

void FixPrint::injectDependencies(Engine& engine_) {
	Ref::injectDependencies(engine_);
	particles = engine_.getParticlesForUpdate().get();
	checkPointer(particles, "particles");
}

void FixPrint::init()
{
	if (variable == "x") {
		inData = particles->getXData();
	}
	else if (variable == "v") {
		inData = particles->getVData();
	}
	else if (variable == "f") {
		inData = particles->getFData();
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