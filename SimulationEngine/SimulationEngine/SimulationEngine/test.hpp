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
#include "DamperField.h"
#include "FixPrint.h"
#include "FixList.h"
#include "Box.h"
#include "Run.h"
#include "Error.h"


/*
 * using commands for std functions
 */
using std::vector, std::array;
using std::make_unique, std::unique_ptr;


/*
 * Mocked types
 */

class MockedBox : public Box {
public:
	MockedBox() :
		Box{},
		min{ std::array<double,3>{0.0,0.0,0.0} },
		max{ std::array<double,3>{0.0,0.0,0.0} }
	{
	}
	MockedBox(std::array<double, 3> min_, std::array<double, 3> max_) :
		Box{},
		min{ std::move(min_) },
		max{ std::move(max_) }
	{
	}
	Error* getError() {
		return error;
	}
private:
	std::array<double, 3> min, max;
};


class MockedForceField : public ForceField {
public:
	MockedForceField(std::vector<double> forces_) :
		ForceField{},
		force{ std::move(forces_) }
	{
	}
	MockedForceField() :
		ForceField{},
		haveIupdatedForces{ true }
	{
	}
	void calculate_pair(std::array<double, 3>& dist_,
		double* fforce_, double& energy_) override
	{
		if (haveIupdatedForces)
			return;
		int nmax, nlocal;
		particles->getNmaxNlocal(nmax, nlocal);
		int nlocalForces = static_cast<int>(force.size() / 3);
		if (nlocal != force.size())
			throw std::invalid_argument("Wrong force vector size!");
		for (int i = 0; i < nlocal; i++) {
			particles->F(i, 0) = force[3 * i];
			particles->F(i, 1) = force[3 * i + 1];
			particles->F(i, 2) = force[3 * i + 2];
		}
		haveIupdatedForces = true;
	}
private:
	std::vector<double> force;
	bool haveIupdatedForces = false;
};

class MockedForceField2 : public ForceField {
public:
	MockedForceField2() :
		ForceField{}
	{
	}

	virtual void init() override {
		nInit++;
	}

	virtual void update() override {
		nUpdates++;
	}

	int nInit = 0;
	int nUpdates = 0;
};

// MockedNeighbor class
class MockedNeighbor : public Neighbor {
public:
	MockedNeighbor(
		int nNeigh_,
		std::vector<int>& neighListVec_,
		std::vector<int>& firstNeighVec_,
		std::vector<int>& numNeighVec_) :
		Neighbor{},
		nNeigh{ nNeigh_ },
		neighListVec{ std::move(neighListVec_) },
		firstNeighVec{ std::move(firstNeighVec_) },
		numNeighVec{ std::move(numNeighVec_) }
	{
	}
	MockedNeighbor() :
		Neighbor{},
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

class MockedNeighbor2 : public Neighbor {
public:
	MockedNeighbor2() :
		Neighbor{} {
	}

	void init() override {
		nInit++;
	}

	void update() override {
		nUpdates++;
	}

	void getNeighborList(int& nNeigh_, int*& neighList_, int*& firstNeigh_, int*& numNeigh_) override
	{
		throw std::invalid_argument("This mock type does not support the getNeighborList function!");
	}

	int nInit = 0;
	int nUpdates = 0;
};

// Mocked Particles class
class MockedParticles : public Particles {
public:
	MockedParticles() :
		Particles{}
	{
	}
	MockedParticles(
		int nmax_,
		std::vector<double>& x_,
		std::vector<double>& v_,
		std::vector<double>& f_,
		std::vector<double>& r_,
		std::vector<double>& m_) :
		Particles{}
	{
		nmax = nmax_;
		x = std::move(x_);
		v = std::move(v_);
		f = std::move(f_);
		r = std::move(r_);
		m = std::move(m_);
	}
	Error* getError()
	{
		return error;
	}

private:
};


// mocked integrator to do some updates on the particles object
class MockedIntegrator : public Integrator
{
public:
	MockedIntegrator() :
		Integrator{}
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
	MockedIntegrator2() :
		Integrator{}
	{
	}

	void post_force() override
	{
		nTimes++;
	}

	int nTimes = 0;
};

class MockedFix : public Fix {
public:
	MockedFix(FixMask mask_, std::string id_) :
		Fix{ mask_, id_ }
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


/*
* Test fixtures
*/

// EngineFixture used for integration and regression tests
class EngineFixture {
public:
	EngineFixture(std::vector<std::unique_ptr<Ref>>& inputObjs) {
		engine_ptr = std::make_unique<Engine>();
		for (auto& inputObj : inputObjs)
			engine_ptr->setItem(std::move(inputObj));
		// since we first inputted the items
		// we can now inject dependencies
		engine_ptr->injectDependencies();
		engine_ptr->init();
	}
	EngineFixture(std::vector<std::string>& args_) :
		commands{ std::move(args_) }
	{}

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
		// getting the run command
		auto& run_ptr = engine_ptr->getRunCommand();
		// setting up the simulation
		run_ptr->setup();
		// running the simualation
		run_ptr->start();
	}

	// building and returning the engine
	// should be removed run and return should be the same
	// may be it is better to return the engine_ptr by ref
	// it is dangerious... not it is not good at all.
	std::unique_ptr<Engine> returnEngine() {
		checkPtrs();
		return std::move(engine_ptr);
	}

	std::unique_ptr<Engine>& getEngineRef() {
		return engine_ptr;
	}

	std::unique_ptr<Fix>& returnFixById(const std::string id_) {
		// an status should be added to the engine to check 
		// if it is run or not and if it is not run 
		// it runs.
		checkPtrs();
		return engine_ptr->returnFixById(id_);
	}
};

class Test_integrator_fixture {
public:
	Test_integrator_fixture(
		double& dt_,
		std::unique_ptr<Integrator>& integrator_,
		std::vector<double>& x_,
		std::vector<double>& v_,
		std::vector<double>& f_,
		std::vector<double>& r_,
		std::vector<double>& m_) :
		dt{ dt_ },
		integrator{ std::move(integrator_) },
		x{ std::move(x_) },
		v{ std::move(v_) },
		f{ std::move(f_) },
		r{ std::move(r_) },
		m{ std::move(m_) }
	{
	}
	void setup()
	{
		// number of particles
		int nlocal = x.size();
		int nmax = x.size();
		// particles
		mockedParticles = std::make_unique<MockedParticles>(nmax, x, v, f, r, m);
		// the timestep
		constexpr double dt = 1.0;
		// setting the timestep
		integrator->setDt(dt);
		// registering Particles and Integrator objects
		engine.setItem(std::move(integrator));
		engine.setItem(std::move(mockedParticles));
		// injecting dependencies
		engine.injectDependencies();
	}
	std::unique_ptr<Integrator>& return_integrator()
	{
		// returning the integrator object
		auto& integratorRef = engine.getIntegrator();
		return integratorRef;
	}
	std::unique_ptr<Particles>& return_particles()
	{
		auto& particlesRef = engine.getParticlesForUpdate();
		return particlesRef;
	}

private:
	double dt;
	Engine engine;
	std::unique_ptr<Particles> mockedParticles;
	std::unique_ptr<Integrator> integrator;
	std::vector<double> x, v, f, r, m;
};


