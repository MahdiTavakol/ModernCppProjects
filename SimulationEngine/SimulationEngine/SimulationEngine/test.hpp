#include "Particles.h"
#include "Engine.h"
#include "EngineFactory.h"
#include "Integrator.h"
#include "EulerIntegrator.h"
#include "Forcefield.h"
#include "ColliIntegrator.h"
#include "Neighbor.h"
#include "SimpleNeighbor.h"
#include "SemiIntegrator.h"
#include "SpringField.h"
#include "FixPrint.h"
#include "FixList.h"
#include "Box.h"
#include "Run.h"
#include "Error.h"

using std::vector, std::array;
using std::make_unique, std::unique_ptr;

// EngineFixture used for integration and regression tests
class EngineFixture {
public:
	EngineFixture(std::vector<std::string>& args_) :
		commands{ std::move(args_) }
	{
	}

	// std::string commands vector
	std::vector<std::string> commands;
	// the factory_ptr
	std::unique_ptr<EngineFactory> factory_ptr = nullptr;
	// the engine pointer
	std::unique_ptr<Engine> engine_ptr = nullptr;
	// checking if we have created those pointers before
	void checkPtrs()
	{
		if (engine_ptr == nullptr) {
			if (factory_ptr == nullptr)
				factory_ptr = std::make_unique<EngineFactory>(commands);
			engine_ptr = factory_ptr->returnEngine();
		}
	}
	// running the engine
	// should return the engine
	void runEngine() {
		// it possibly has been returned
		checkPtrs();
		// setting up the simulation
		engine_ptr->setupSim();
		// running the simualation
		engine_ptr->runSim();
	}

	// building and returning the engine
	// should be removed run and return should be the same
	// may be it is better to return the engine_ptr by ref
	// it is dangerious... not it is not good at all.
	std::unique_ptr<Engine> returnEngine() {
		checkPtrs();
		return std::move(engine_ptr);
	}

	std::unique_ptr<Fix>& returnFixById(const std::string id_) {
		// an status should be added to the engine to check 
		// if it is run or not and if it is not run 
		// it runs.
		checkPtrs();
		return engine_ptr->returnFixById(id_);
	}
};

class MockedBox : public Box {
public:
	MockedBox(Engine& engine_,
		std::array<double, 3> min_, std::array<double, 3> max_) :
		Box{ engine_ },
		min{ std::move(min_) },
		max{ std::move(max_) }
	{
	}
private:
	std::array<double, 3> min, max;
};

class MockedForceField : public ForceField {
public:
	MockedForceField(Engine& engine_,
		std::vector<double> forces_) :
		ForceField{ engine_ },
		force{ std::move(forces_) }
	{
	}
	MockedForceField(Engine& engine_) :
		ForceField{ engine_ },
		haveIupdatedForces{ true }
	{
	}
	void calculate_pair(std::array<double, 3>& dist_,
		double* fforce_, double& energy_) override
	{
		if (haveIupdatedForces)
			return;
		auto& box = engine().getBox();
		auto& P = engine().getParticlesForUpdate();
		int nmax, nlocal;
		P->getNmaxNlocal(nmax, nlocal);
		int nlocalForces = static_cast<int>(force.size() / 3);
		if (nlocal != force.size())
			throw std::invalid_argument("Wrong force vector size!");
		for (int i = 0; i < nlocal; i++) {
			P->F(i, 0) = force[3 * i];
			P->F(i, 1) = force[3 * i + 1];
			P->F(i, 2) = force[3 * i + 2];
		}
		haveIupdatedForces = true;
	}
private:
	std::vector<double> force;
	bool haveIupdatedForces = false;
};

// MockedNeighbor class
class MockedNeighbor : public Neighbor {
public:
	MockedNeighbor(Engine& engine_,
		int nNeigh_,
		std::vector<int>& neighListVec_,
		std::vector<int>& firstNeighVec_,
		std::vector<int>& numNeighVec_) :
		Neighbor{ engine_ },
		nNeigh{ nNeigh_ },
		neighListVec{ std::move(neighListVec_) },
		firstNeighVec{ std::move(firstNeighVec_) },
		numNeighVec{ std::move(numNeighVec_) }
	{
	}
	MockedNeighbor(Engine& engine_) :
		Neighbor{ engine_ },
		nNeigh{ 0 }
	{
	}
	~MockedNeighbor() = default;
	void init() override {}
	void getNeighborList(int& nNeigh_, int*& neighList_, int*& firstNeigh_, int*& numNeigh_) override {
		nNeigh_ = nNeigh;
		neighList_ = neighListVec.data();
		firstNeigh_ = firstNeighVec.data();
		numNeigh_ = numNeighVec.data();
	}

private:
	int nNeigh;
	vector<int> neighListVec, firstNeighVec, numNeighVec;
};

// Mocked Particles class
class MockedParticles : public Particles {
public:
	MockedParticles(Engine& engine_,
		int nmax_,
		std::vector<double>& x_,
		std::vector<double>& v_,
		std::vector<double>& f_,
		std::vector<double>& r_,
		std::vector<double>& m_) :
		Particles{ engine_ }
	{
		nmax = nmax_;
		x = std::move(x_);
		v = std::move(v_);
		f = std::move(f_);
		r = std::move(r_);
		m = std::move(m_);
	}

private:
};


// mocked integrator to do some updates on the particles object
class MockedIntegrator : public Integrator
{
public:
	MockedIntegrator(Engine& engine_) :
		Integrator{ engine_ }
	{
	}
	void updateX() {
		positionUpdate();
	}
	void updateV() {
		velocityUpdate();
	}
};

// mocked integrator to count how many times it was called.
class MockedIntegrator2 : public Integrator {
public:
	MockedIntegrator2(Engine& engine_) :
		Integrator{ engine_ }
	{}

	void init() override {
		nInit++;
	};
	void setup() override {
		nSetup++;
	}
	void neighbor_build() override {};
	void initial_integrate() override {};
	void pre_force() override {};
	void force() override {
		nTimes++;
	};
	void post_force() override {};
	void final_integrate() override {};

	int nTimes = 0;
	int nInit = 0;
	int nSetup = 0;

};



class MockedFix : public Fix {
public:
	MockedFix(Engine& engine_, FixMask mask_, std::string id_) :
		Fix{ engine_, mask_, id_ }
	{
	}

	void init() override {
		nInit++;
	}
	void setup() override {
		nSetup++;
	}
	void initial_integrate() override {
		if (mask & INIT_INTEGRATE)
			nTimes++;
	}
	void pre_force()  override {
		if (mask & PRE_FORCE)
			nTimes++;
	}
	void post_force()  override {
		if (mask & POST_FORCE)
			nTimes++;
	}
	void final_integrate()  override {
		if (mask & FINAL_INTEGRATE)
			nTimes++;
	}

	int nInit = 0;
	int nSetup = 0;
	int nTimes = 0;
};
