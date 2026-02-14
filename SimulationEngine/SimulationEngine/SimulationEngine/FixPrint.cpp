#include "FixPrint.h"
#include "Particles.h"

FixPrint::FixPrint(Engine& engine_,
	std::string id_,
	int nevery_ ,
	int mask_,
	string variable_,
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
	array<double, 3> out =
	{ inData[3 * particleId], inData[3 * particleId + 1] , inData[3 * particleId + 2] };

	data.push_back(out);
}

const vector<array<double, 3>>& FixPrint::getOutputVector() const {
	return data;
}