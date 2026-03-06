#pragma once
#include "Engine.h"
#include "Ref.h"
#include "Particles.h"
#include "Box.h"


struct transferedData {
	std::vector<std::array<double, 3>> xData;
	std::vector<std::array<double, 3>> vData;
	std::vector<std::array<double, 3>> fData;
	std::vector<double> mData;
	std::vector<double> rData;
};


class Communicator : public Ref {
public:
	Communicator();
	Communicator(std::vector<std::string>& args_);
	Communicator(const int& myId_, const int& size_, const double& skin_ = 0.0);
	Communicator(const int& myId_, const std::array<int, 3>& size_, const double& skin_ = 0.0);
	void injectDependencies(Engine& engine_) override;
	void init();

	void forward_particle(const std::array<int,3>& dir_,
		                  transferedData& trandata_);
	void updateGhosts(const std::array<int,3>& dir_, const transferedData& trandata_);

	
protected:
	// box ref
	Box* box = nullptr;
	// Particles ref
	Particles* particles = nullptr;
	// we need a pointer to the engine
	Engine* engine = nullptr;
	// resetting the particles
	void resetParticles();
	// the box dimensions
	std::array<double, 3> myMin = { 0,0,0 }, myMax = { 0,0,0 };

	// number of particles
	int nlocal = 0, nmax = 0, nghosts = 0;
	// ghost particles id
	std::vector<int> ghostsXLo, ghostsXHi, ghostsYLo, ghostsYHi, ghostsZLo, ghostsZHi, ghostIds;
	// interior particles id (to serve as ghosts for neighboring ranks
	std::vector<int> interXLo, interXHi, interYLo, interYHi, interZLo, interZHi;
	// skin
	double skin = 0.0;
	// communicator id
	int myId = 0;
	// communicator configuration
	std::array<int, 3> sizeArray = { 2,2,1 };

	// Forward_comm Partners
	int forward_partner[3][2];

	// Reverse_comm Partners
	int reverse_partner[3][2];

};
