#pragma once

#include "Particles.h"
#include "Communicator.h"
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

#include "catch_amalgamated.hpp"
#include <algorithm>


/*
 * using commands for std functions
 */
using std::vector, std::array;
using std::make_unique, std::unique_ptr;


/*
* Non default matcher for X, V and F arrays
* 
*/


class Array3DMatcher : public Catch::Matchers::MatcherGenericBase
{
	double tol;
	int nLocal, nGhost;
	typedef struct
	{
		double x;
		double y;
		double z;
	}
	dataStruct;
	std::vector<dataStruct> expected;
	double expectedSize;

	mutable std::vector<int> badIndexes;



public:
	Array3DMatcher(double* expected_, int nLocal_, int nGhost_, double tol_) :
		tol{ tol_ }, nLocal{ nLocal_ }, nGhost{ nGhost_ }, expectedSize{ 0.0 }
	{
		for (int i = 0; i < nLocal + nGhost; i++)
		{
			dataStruct datai = { expected_[3 * i],expected_[3 * i + 1] ,expected_[3 * i + 2] };
			expected.push_back(datai);
			expectedSize += datai.x * datai.x + datai.y * datai.y + datai.z * datai.z;
		}

		expectedSize = std::sqrt(expectedSize);

		std::sort(expected.begin(), expected.begin() + nLocal,
			[](const dataStruct& a, const dataStruct& b) {
				if (std::abs(a.x - b.x) >= 1e-6) return a.x < b.x;
				if (std::abs(a.y - b.y) >= 1e-6) return a.y < b.y;
				return a.z < b.z;
			});
		std::sort(expected.begin() + nLocal, expected.end(),
			[](const dataStruct& a, const dataStruct& b) {
				if (std::abs(a.x - b.x) >= 1e-6) return a.x < b.x;
				if (std::abs(a.y - b.y) >= 1e-6) return a.y < b.y;
				return a.z < b.z;
			});
	}
	Array3DMatcher(double* expected_, int nLocal_, double tol_) :
		Array3DMatcher{expected_,nLocal_,0,tol_}
	{}

	bool match(double* value_) const
	{
		std::vector<dataStruct> valueVector;

		for (int i = 0; i < nLocal + nGhost; i++) {
			dataStruct datai = { value_[3 * i],value_[3 * i + 1],value_[3 * i + 2] };
			valueVector.push_back(datai);
		}
		std::sort(valueVector.begin(), valueVector.begin() + nLocal,
			[](const dataStruct& a, const dataStruct& b) {
				if (std::abs(a.x - b.x) >= 1e-6) return a.x < b.x;
				if (std::abs(a.y - b.y) >= 1e-6) return a.y < b.y;
				return a.z < b.z;
			});
		std::sort(valueVector.begin() + nLocal, valueVector.end(),
			[](const dataStruct& a, const dataStruct& b) {
				if (std::abs(a.x - b.x) >= 1e-6) return a.x < b.x;
				if (std::abs(a.y - b.y) >= 1e-6) return a.y < b.y;
				return a.z < b.z;
			});


		double diff = 0.0;

		for (int i = 0; i < nLocal + nGhost; i++) {
			double diffxi = valueVector[i].x - expected[i].x;
			double diffyi = valueVector[i].y - expected[i].y;
			double diffzi = valueVector[i].z - expected[i].z;

			double diffi = diffxi * diffxi + diffyi * diffyi + diffzi * diffzi;


			if (std::sqrt(diffi) > tol * expectedSize/static_cast<double>(nLocal + nGhost)) {
				badIndexes.push_back(i);
				/*std::cout <<
					i << "," << value[i].x
					  << "," << value[i].y
					  << "," << value[i].z
					  << "," << expected[i].x
					  << "," << expected[i].y
					  << "," << expected[i].z
					  << std::endl;
					  */
			}
			diff += diffi;

		}
		diff = std::sqrt(diff);

		return diff <= tol * expectedSize;
		
	}

	std::string describe() const override
	{
		std::string message = "Comparing two 3D arrays with the tolerance of " + std::to_string(tol);

		if (!badIndexes.empty()) {
			message += "\nMismatching indexes:";
			for (const auto& index : badIndexes)
				message += "\n" + std::to_string(index);
		}
		return message;
	}

};

/* Non default matcher for r and m vectors */

class Array1DMatcher : public Catch::Matchers::MatcherGenericBase
{
	double tol;
	int nLocal, nGhost;
	std::vector<double> expected;
	double expectedSize;


public:
	Array1DMatcher(double* expected_, int nLocal_, int nGhost_, double tol_) :
		tol{ tol_ }, nLocal{ nLocal_ }, nGhost{nGhost_}
	{
		expectedSize = 0.0;
		for (int i = 0; i < nLocal + nGhost; i++)
		{
			expected.push_back(expected_[i]);
			expectedSize += std::abs(expected_[i]);
		}

		std::sort(expected.begin(), expected.begin()+nLocal);
		std::sort(expected.begin() + nLocal, expected.end());
	}

	Array1DMatcher(double* expected_, int nLocal_, double tol_):
		Array1DMatcher{expected_,nLocal_,0,tol_}
	{}


	bool match(double* value_) const
	{
		double diff = 0.0;
		std::vector<double> valueVector;

		for (int i = 0; i < nLocal + nGhost; i++) {
			valueVector.push_back(value_[i]);
		}
		std::sort(valueVector.begin(), valueVector.begin() + nLocal);
		std::sort(valueVector.begin()+ nLocal, valueVector.end());

		for (int i = 0; i < nLocal + nGhost; i++) {
			diff += std::abs(valueVector[i] - expected[i]);
		}

		return diff <= tol * expectedSize;
	}

	std::string describe() const override
	{
		return "Comparing two 1D arrays with the tolerance of " + std::to_string(tol);
	}

};

class Array1DMatcherInt : public Catch::Matchers::MatcherGenericBase
{
	int nLocal, nGhost;
	std::vector<int> expectedVec;
	mutable vector<int> badIndexes;
public:
	Array1DMatcherInt(int* expected_, int nLocal_, int nGhost_ = 0) :
		nLocal{ nLocal_ }, nGhost{ nGhost_ }
	{
		for (int i = 0; i < nLocal + nGhost; i++)
			expectedVec.push_back(expected_[i]);

		std::sort(expectedVec.begin(), expectedVec.begin() + nLocal);
		std::sort(expectedVec.begin() + nLocal, expectedVec.end());

	}

	bool match(int* value_) const
	{
		int misMatchNum = 0;
		// I do not want to change the original values order
		std::vector<int> valueVec;
		for (int i = 0; i < nLocal + nGhost; i++)
			valueVec.push_back(value_[i]);

		std::sort(valueVec.begin(), valueVec.begin() + nLocal);
		std::sort(valueVec.begin() + nLocal, valueVec.end());

		for (int i = 0; i < nLocal + nGhost; i++)
		{
			if (valueVec[i] != expectedVec[i])
			{
				misMatchNum++;
				badIndexes.push_back(i);
			}
		}

		return (misMatchNum == 0);
	}

	std::string describe() const override
	{
		std::string message = "Comparing two 1D int arrays";

		if (!badIndexes.empty()) {
			message += "\nMismatching indexes:";
			for (const auto& index : badIndexes)
				message += "\n" + std::to_string(index);
		}
		return message;
	}
};

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
		nmax = 0;
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
		nlocal = x_.size()/3;
		x = std::move(x_);
		v = std::move(v_);
		f = std::move(f_);
		r = std::move(r_);
		m = std::move(m_);
		resetIds();
	}
	void resetParticles(
		int nmax_,
		std::vector<int>& id_,
		std::vector<double>& x_,
		std::vector<double>& v_,
		std::vector<double>& f_,
		std::vector<double>& r_,
		std::vector<double>& m_
	)
	{
		resetParticles(nmax_, 0,id_, x_, v_, f_, r_, m_);
	}
	void resetParticles(
		int nmax_,
		int nghost_,
		std::vector<int>& id_,
		std::vector<double>& x_,
		std::vector<double>& v_,
		std::vector<double>& f_,
		std::vector<double>& r_,
		std::vector<double>& m_
	)
	{
		nmax = nmax_;
		nlocal = x_.size() / 3 - nghost_;
		id = std::move(id_);
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
		for (auto& inputObj : inputObjs) {
			engine_ptr->setItem(std::move(inputObj));
		}
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
			/* 
			 * Checking which one the contructors 
			 * has been used to create the engine
			 */
			// the first branch.. As we used the std::move in C'tr we do not have the original parameters
			if (commands.empty())
				throw std::invalid_argument("An unrecoverable error!");
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

	std::unique_ptr<Engine>& returnEngineRef() {
		return engine_ptr;
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
		auto nlocal = x.size();
		auto nmax = x.size();
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




