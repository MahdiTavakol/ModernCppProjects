#pragma once
#include "Engine.h"
#include "Ref.h"
#include "Particles.h"
#include "Box.h"


class Communicator : public Ref {
public:
	Communicator();
	Communicator(std::vector<std::string>& args_);
	Communicator(const int& myId_, const int& size_, const double& skin_ = 0.0);
	Communicator(const int& myId_, const std::array<int, 3>& size_, const double& skin_ = 0.0);
	virtual ~Communicator();
	void injectDependencies(Engine& engine_) override;
	void init();



	int sendGhosts(const int& partnerRank_,
		                  std::vector<double>& trandata_);
	void recvGhosts(std::vector<double>& trandata_);

	int sendParticles(const int& partnerRank_,
		std::vector<double>& trandata_);
	void recvParticles(std::vector<double>& trandata_);
	
	// getting the destination array for the exchange
	// xlo xhi ylo yhi zlo zhi
	std::array<int,6> returnExchangeDests();
	// xlo, ylo, zlo --->>> left
	//std::array<int, 3> returnLeftProcs();
	// xhi, yhi, zhi --->>> right
	// s
	// getting min max
	void getMinMax(std::array<double, 3>& min_, std::array<double, 3>& max_) {
		min_ = myMin; max_ = myMax;
	}
	// resetting the owned particles
	void resetOwned();

	// resetting the ghost particles
	void resetGhosts();
	// resetting the particles
	void resetParticles();


	// the proxy function for doing the particle and ghost exchange
	void exchangeParticleGhost();




protected:
	// box ref
	Box* box = nullptr;
	// Particles ref
	Particles* particles = nullptr;
	// we need a pointer to the Comm_strategy object of the Engine
	Comm_strategy* comm_strategy = nullptr;
	// we need a pointer to the engine
	Engine* engine = nullptr;
	// the box dimensions
	std::array<double, 3> myMin = { 0,0,0 }, myMax = { 0,0,0 };

	// helper functions used to determine particles to be transfered to the neighboring ranks
	bool isExterXLo(double x, double y, double z) { return x < myMin[0]; }
	bool isExterXHi(double x, double y, double z) { return x > myMax[0]; }
	bool isExterYLo(double x, double y, double z) { return y < myMin[1]; }
	bool isExterYHi(double x, double y, double z) { return y > myMax[1]; }
	bool isExterZLo(double x, double y, double z) { return z < myMin[2]; }
	bool isExterZHi(double x, double y, double z) { return z > myMax[2]; }
	// helper functions to determine particles to send as ghosts to the neighboring ranks
	bool isInterXLo(double x, double y, double z) { return x < myMin[0] + skin; }
	bool isInterXHi(double x, double y, double z) { return x > myMax[0] - skin; }
	bool isInterYLo(double x, double y, double z) { return y < myMin[1] + skin; }
	bool isInterYHi(double x, double y, double z) { return y > myMax[1] - skin; }
	bool isInterZLo(double x, double y, double z) { return z < myMin[2] + skin; }
	bool isInterZHi(double x, double y, double z) { return z > myMax[2] - skin; }
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


	// the information for exchanging particles between ranks
	// the neighboring ranks
	int neighbor_ranks[3][2];

	std::vector<double> exchangeMessages[6];
	// adding particles to the exchanged messages
	void addParticles2ExchangeMessages(const int& loc, const int& particleId);


};
