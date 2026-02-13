#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>

using std::vector, std::array;
using std::make_unique, std::unique_ptr;
using std::reference_wrapper;

class Engine;

class Box {
public:
	Box(Engine& engine_,
		const array<int, 3>& min_,
		const array<int, 3>& max_) :
		engineRef{ engine_},
		min{min_}, max{max_}
	{}
private:
	reference_wrapper<Engine> engineRef;
	array<int, 3> min, max;
};

class Particles {
public:
	Particles(Engine& engine_, const int& nmax_) :
		engineRef{engine_}, 
		nmax{ nmax_ }
	{
		positions.reserve(nmax);
	}
	void addParticle(array<int, 3> newPosition_)
	{
		//check the boundary conditions
		positions.push_back(newPosition_);
	}

private:
	reference_wrapper<Engine> engineRef;
	int nmax;
	vector<array<int, 3>> positions;
};

class Engine {
public:
	Engine(const int& nmax, array<int, 3> min, array<int, 3> max):
		box{make_unique<Box>(*this,min,max)},
		particles{make_unique<Particles>(*this,nmax)}
	{}
	void resetBox(unique_ptr<Box>& box_) {
		box = std::move(box_);
	}
	void resetParticles(unique_ptr<Particles> particles_) {
		particles = std::move(particles_);
	}

private:
	unique_ptr<Box> box;
	unique_ptr<Particles> particles;
	int nmax;
};

TEST_CASE("Starting each class with minimal input args!") {
	// constant variables
	int nmax = 10;
	array<int, 3> min = { 0,0,0 };
	array<int, 3> max = { 10,10,10 };

	// class engine
	Engine engine(nmax,min,max);

	// class box
	Box box(engine, min, max);

	// particles
	Particles particles(engine, nmax);


	// adding new particles
	vector<array<int, 3>> positions;
	positions.push_back({ 1,1,1 });
	positions.push_back({ 2,3,5 });
	positions.push_back({ 5,8,2 });
	for (const auto& position : positions)
		particles.addParticle(position);
}