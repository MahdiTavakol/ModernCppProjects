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
	void setInteriorParticles() {}


	std::array<std::vector<int>*,2> setInterVec4NeighborRank(const int& partnerRank_);
	std::vector<int>* setExterVec4NeighborRank(const int& partnerRank_);
	int sendGhosts(const int& partnerRank_,
		                  std::vector<double>& trandata_);
	void recvGhosts(std::vector<double>& trandata_);

	int sendParticles(const int& partnerRank_,
		std::vector<double>& trandata_);
	void recvParticles(std::vector<double>& trandata_);
	

	// getting the nDests used to see if there is no more data exchange
	int getNDests();
	// getting the destination array for the exchange
	// xlo xhi ylo yhi zlo zhi
	std::array<int,6> returnExchangeDests();
	// getting the data for the exchanged particles 
	std::vector<double>* sendExchangeParticles();
	// receving the exchanged particle data
	void recvExchangeParticles(std::vector<double>& messages);
	// getting min max
	void getMinMax(std::array<double, 3>& min_, std::array<double, 3>& max_) {
		min_ = myMin; max_ = myMax;
	}
	// resetting the owned particles
	void resetOwned();
	// resetting the interior particles
	void resetInterior();
	// clearing the ghost particles to be sent to neighboring ranks
	void clearGhostInterior();
	// resetting the interior particles (from ghost particles)
	void resetGhostInterior();

	// adding to the ghost particles to be sent to the neighboring ranks
	void addGhostInterior(const int& i_);
	// resetting the ghost particles
	void resetGhosts();
	// resetting the particles
	void resetParticles();
	// getting interXLo
	std::vector<int>& getInterXLo()
	{
		return interXLo;
	}


protected:
	// box ref
	Box* box = nullptr;
	// Particles ref
	Particles* particles = nullptr;
	// we need a pointer to the engine
	Engine* engine = nullptr;
	// the box dimensions
	std::array<double, 3> myMin = { 0,0,0 }, myMax = { 0,0,0 };

	// interior particles id (to serve as ghosts for neighboring ranks)
	std::vector<int> interXLo, interXHi, interYLo, interYHi, interZLo, interZHi;
	// interior ghost ids (to serve as ghosts for neighboring ranks)
	std::vector<int> interGhostXLo, interGhostXHi, interGhostYLo, interGhostYHi, interGhostZLo, interGhostZHi;
	// particle ids set for sending to a neighboring rank (out of this rank borders)
	std::vector<int> exterXLo, exterXHi, exterYLo, exterYHi, exterZLo, exterZHi;
	bool isExterXLo(double x, double y, double z) { return x < myMin[0]; }
	bool isExterXHi(double x, double y, double z) { return x > myMax[0]; }
	bool isExterYLo(double x, double y, double z) { return y < myMin[1]; }
	bool isExterYHi(double x, double y, double z) { return y > myMax[1]; }
	bool isExterZLo(double x, double y, double z) { return z < myMin[2]; }
	bool isExterZHi(double x, double y, double z) { return z > myMax[2]; }
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
	//
	int  nDests = 0;
	std::vector<double> exchangeMessages[6];
	void initExchangeData();
	// adding particles to the exchanged messages
	void addParticles2ExchangeMessages(const int& loc, const int& particleId);


};
