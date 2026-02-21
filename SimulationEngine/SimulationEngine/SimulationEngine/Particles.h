#pragma once

#include "Engine.h"
#include "Ref.h"

#include <vector>
#include <array>


class Particles : public Ref {
public:
	Particles(Engine& engine_);
	Particles(Engine& engine_, int nmax_);
	Particles(Engine& engine_, std::vector<std::string> args_);
	void addParticle(std::array<double, 3> newX_,
		std::array<double, 3> newV_ = { 0.0,0.0,0.0 },
		std::array<double, 3> newF_ = { 0.0,0.0,0.0 },
		double newM_ = 10.0);
	void getNmaxNlocal(int& nmax_, int& nlocal_) const;
	// get atom i position
	void getAtomVec(const int& i,
		std::array<double, 3>& xi,
		std::array<double, 3>& vi,
		std::array<double, 3>& fi,
		double& mi);
	// used int* const on purpose to avoid the 
	// memory managed by vector to be destructed or changed
	double* const getXData() { return x.data(); }
	double* const getVData() { return v.data(); }
	double* const getFData() { return f.data(); }
	double* const getMData() { return m.data(); }

private:
	int nmax;
	std::vector<double> x;
	std::vector<double> v;
	std::vector<double> f;
	std::vector<double> m;
};

